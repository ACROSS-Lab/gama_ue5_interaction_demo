/**
* Name: GridModel
* Based on the internal empty template. 
* Author: THANTHUY
* Tags: 
*/


model GridModel

/* Insert your model definition here */
global skills:[network]{
	
	//UE connection variables
	int port <- 8000;
	unknown ue_client;
	bool initialized <- false;
	
	//constants
	int empty_building_type <- 0;
	int house_type 			<- 1;
	int office_type 		<- 2;
	int num_lanes_roads <- 3;
	
	// Simulation parameters
	float step <- 3#second;
	int office_capacity <- 50 parameter:"Office capacity";
	int house_capacity <- 30 parameter:"House capacity";
	
	list available_office <- [];
	graph road_network;
	map<road,float> new_weights;
	geometry shape <- square(30*8);
	
	float lane_width <- 1.5;
	
	
	
	init{	
		
		create road from: clean_network((union(environment collect each.shape.contour)).geometries,0.1,true,true)  {
			num_lanes <- num_lanes_roads;
			create road {
				shape <- line(reverse(myself.shape.points));
				num_lanes <- num_lanes_roads;
			}
		}
		graph tmp <- as_edge_graph(road);
		loop pt over: tmp.vertices {
			create intersection with:(location:pt.location);
		
		}
		
			//build the graph from the roads and intersections
		road_network <- as_driving_graph(road,intersection);
		
		
		//initialize with empty buildings everywhere
		loop i from:0 to:7 {
			loop j from:0 to:7 {
				let ev <- environment[i,j];
				empty_building b;
				
				create empty_building {
					location <- ev.location;
					shape <- ev.shape;
					b <- self;
					id <- i * 8 + j;
				}	
				if (flip(2/3.0)) {
					house h <- change_empty_building_to_house(b);
					if flip(1.0/2){
						let o <- change_house_to_office(h);
					}
				}
			}	
		}
	}

	action send_init_data {
		// initialize server
		do connect protocol:"tcp_server" port:port raw:true with_name:"server" force_network_use:true;
		
		write "waiting for the client to send a connection confirmed message"; 
	    loop while: !has_more_message()  { 
	        do fetch_message_from_network;
	    }
	    // we set the client
	    loop while: has_more_message() {
	        message s <- fetch_message();
	        write "cycle: " + cycle + " received from client: " + s.contents;
	    	ue_client <- s.sender;
	    }	
	    
	    // We send every building and agents to UE
	    do send_world;
	    
	}
	
	action send_world {
		
		map to_send;
		let buildings <- 64 list_with 0;
		let people <- [];
		loop b over:empty_building + office + house {
			buildings[b.id] <- b.type;
		}
		loop p over:inhabitant {
			//only send the ones on the road
			if [p.office_location,p.house_location] none_matches (each != nil and each.x = p.location.x and each.y = p.location.y) {
				people <+ p.to_array();				
			}
		}
		to_send <+ "building"::buildings;
		to_send <+ "people"::people;
		to_send <+ "date"::""+current_date;
		
		//write "sending: " + as_json_string(to_send);
		if ue_client = nil 
		{
			write "no client to send to";
		}
		else {
			do send to:ue_client contents:as_json_string(to_send)+'\n' ;			
		}
		
	}

	reflex send_update_to_ue 
	{
		
		if ! initialized {
			do send_init_data;
			initialized <- true;
		}
		else {
			do send_world;			
		}
	}


	reflex read_ue_messages when:has_more_message() {
	  loop while: has_more_message() {
	        message s <- fetch_message();
	        write "cycle: " + cycle + " received from client: " + s.contents;
	    	ue_client <- s.sender; // updates the client
	    	if s.contents != "connected" {
	    		let answer <- map(s.contents);
	    		if int(answer["type"]) = house_type{
	    			let h <- first(house select (each.id = int(answer["id"])));
	    			write "change house to office " + h;
	    			if h != nil {
		    			do change_house_to_office(h);	    				
	    			}
	    		}
	    		else if int(answer["type"]) = office_type{
	    			let o <- first(office select (each.id = int(answer["id"])));
	    			write "change office to empty " + o;
	    			if (o != nil){
		    			do change_office_to_empty_building(o);	    				
	    			}
	    		}
	    		else if int(answer["type"]) = empty_building_type{
	    			let e <- first(empty_building select (each.id = int(answer["id"])));
	    			write "change empty to house " + e;	 
	    			if e != nil {
		    			do change_empty_building_to_house(e);	    				
	    			}   			
	    		}
	    	}
	    }	
	}

	//when a road is busy, it turns into yellow & inhabitant's speed decreases
/*	reflex update_road{
//		map<road,float> weights_map <- road as_map (each::(count(inhabitant overlapping each)) / each.shape.perimeter));
//		road_network <- as_edge_graph(road) with_weight weights_map;
		loop i over: road{
			int nb_people_on_road <- length(inhabitant overlapping i);
			if(nb_people_on_road/i.shape.perimeter >= 0.2){
				ask i{
					color <- #red;
					ask inhabitant {
						if self overlaps i{
							speed <- initial_speed * 0.02;
						}
						else {
							speed <- initial_speed;
						}
					}
				}
			}else{
				ask i{
					color <- #black;
				}
			}
			//write 'traffic'+ nb_people_on_road/i.shape.perimeter;
		}
	} */

	reflex update_office_location{
		ask inhabitant{
			if ( office none_matches (each overlaps location)) {
				if (available_office none_matches (each overlaps office_location) ) {
					office_location <- not empty(available_office) ? any_location_in(one_of(available_office))  : nil;					
				}										
			}
		}
	}

	reflex update_available_office{
		loop i over: office{
			if(length(inhabitant overlapping i) < office_capacity and !(i in available_office)){
				available_office <- available_office + i;
			}
			if((i in available_office) and (length(inhabitant overlapping i) >= office_capacity)){
				available_office <- available_office - i;
			}	
		}
	}
	
	office change_house_to_office(house old_house) {
		office o;
		//kill inhabitant belonging to that house
		loop i over: inhabitant{
			if (old_house overlaps i.house_location){
				ask i{
					do die;
				}
			}				
		}
		create office{
			location <- old_house.location;
			shape <- old_house.shape;	
			id <- old_house.id;
			o <- self;
		}
		ask old_house{
			do die;			
		}		
		return o;
	}
	
	empty_building change_office_to_empty_building(office old_office) {
		empty_building b;
		create empty_building {
			location <- old_office.location;
			shape <- old_office.shape;	
			id <- old_office.id;
			b <- self;
		}

		available_office >> old_office;

		ask inhabitant where (each.office_location = old_office.location){
			office_location <- not empty(available_office) ? any_location_in(one_of(available_office)) : nil;
		}
		ask old_office{
			do die;
		}
		return b;
	}
	
	house change_empty_building_to_house(empty_building old_building){
		house h;
		create house{
			location <- old_building.location;
			shape <- old_building.shape;
			id <- old_building.id;
			create inhabitant number: house_capacity{
				location <- any_location_in((myself).shape);
				house_location <- location;
				office_location <- not empty(available_office) ? any_location_in(one_of(available_office)) : nil;
			}				
			h <- self;
		}
		ask old_building {
			do die;
		}
		return h;
	}
	
	action mouse_click{
		environment selected_cell <- first(environment overlapping (#user_location));
		//write selected_cell;
		if selected_cell != nil{
			//If there was nothing we create a house and it's inhabitants
			if(house overlapping (#user_location) = [] and office overlapping (#user_location) = []){
				ask empty_building overlapping #user_location {
					ask world {
						do change_empty_building_to_house(myself);
					}					
				}
			}
			// If there was already a house, we create an office
			else if(house overlapping (#user_location) != []){
				ask house overlapping (#user_location) {
					ask world{
						do change_house_to_office(myself);						
					}
				}
			}
			else {
				//If there was an office we replace with an empty building
				ask office overlapping (#user_location){
					ask world {
						do change_office_to_empty_building(myself);
					}
				}	
			}	
		}
	}
}



species inhabitant skills:[advanced_driving] {
	point office_location <- nil;
	point house_location <- nil;
	point target <- nil;
	rgb color;
	float initial_speed <- rnd(1.5,3.5)#km/#h;	
	point shift_pt <- location ;	
	bool at_home <- true;
	
	init {
		color <- rnd_color(225);
		vehicle_length <- 2.8 #m;
		//car occupies 2 lanes
		num_lanes_occupied <-1;
		max_speed <-30 #km / #h;
				
		proba_respect_priorities <- 0.0;
		proba_respect_stops <- [1.0];
		proba_use_linked_road <- 0.0;

		lane_change_limit <- 2;
		linked_lane_limit <- 0;
		
	}

	/*aspect default{
		draw circle(1) at:shift_pt color: color;
	}*/
	
	aspect default {
		if (current_road != nil) {
			point pos <-shift_pt;
				draw rectangle(vehicle_length, lane_width * num_lanes_occupied) 
				at: pos color: color rotate: heading border: #black;
			draw triangle(lane_width * num_lanes_occupied) 
				at: pos color: #white rotate: heading + 90 ;
		}
	}

/* 	reflex choose_target{	
		if current_date.minute > 30{
			target <- office_location;			
		}	
		else {
			target <- house_location;
		}
//		if(		current_date.hour >= 7 and current_date.hour < 12
//			or 	current_date.hour >= 14 and current_date.hour <= 17 )
//		{
//			target <- office_location;
//		}
//		else if current_date.hour >= 18 and current_date.hour < 23{
//			target <- any_location_in(one_of(empty_building));
//		}
//		else {
//			target <- house_location;						
//		}
	}*/
	
	//choose a random target and compute the path to it
	reflex choose_path when: final_target = nil and flip(0.01) and office_location != nil {
		if at_home{
			target <- office_location;	
			at_home <- false;		
		}	
		else {
			target <- house_location;
			at_home <- true;		
		}
		location <- (intersection closest_to self).location;
		intersection t <-  intersection closest_to target;
		do compute_path graph: road_network target: t; 
		
	}
	reflex move when: final_target != nil {
		do drive;
		//if arrived at target, kill it and create a new car
		if (final_target = nil) {
			do unregister;
			if (at_home) {
				location <-house_location;
			} else {
				location <-office_location;
			}
		//	do die;
		}
		shift_pt <-compute_position()		;
	}
	
	
	point compute_position {
		// Shifts the position of the vehicle perpendicularly to the road,
		// in order to visualize different lanes
		if (current_road != nil) {
			float dist <- (road(current_road).num_lanes - current_lane -
				mean(range(num_lanes_occupied - 1)) - 0.5) * lane_width;
			if violating_oneway {
				dist <- -dist;
			}
		 	
			return location + {cos(heading + 90) * dist, sin(heading + 90) * dist};
		} else {
			return {0, 0};
		}
	}	
	
	map to_json {
	
		return map("id"::int(self), 'x'::shift_pt.x with_precision 1, 'y'::shift_pt.y with_precision 1, "heading"::int(heading)); 
	}
	
	list to_array {
		return [int(self), int(location.x), int(location.y), int(heading)];
	}
}



species intersection skills: [skill_road_node] {
	aspect default{
		draw square(2) color: color;
	}
}
species road skills: [skill_road]{
	rgb color <- #black;
	aspect default{
		draw shape color: color end_arrow:1.0;
	}
}

species building {
	int type <- -1;
	int id;
	map to_json {
		return map("id":: id, "type":: type);
	}
}

species empty_building parent:building{
	
	rgb color <- #grey;
	int type <- empty_building_type;
	

}

species house parent:building{
	rgb color <- #blue;
	int type <- house_type;
}

species office parent:building{
	rgb color <- #orange;
	int type <- office_type;
}

grid environment height:8 width:8 neighbors:4{
	list<environment> neighbors2 <- self neighbors_at 2;

}

experiment grid_model type:gui autorun:true{

	float minimum_cycle_duration <- 0.1#second;



	output{
		
		display main_display type:opengl axes:false{
			//grid environment border: #black;
			species empty_building;
			species intersection;
			species road aspect: default;
			species house;
			species office;
			
			species inhabitant aspect: default;
			event #mouse_down action:mouse_click;
		}	
	}
}


