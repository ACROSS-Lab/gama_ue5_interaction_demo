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
	
	
	// Simulation parameters
	float step <- 1#second;
	
	list available_office <- [];
	graph road_network;
	map<road,float> new_weights;
	geometry shape <- square(30*8);
	
	int office_capacity <- 50 parameter:true;
	int house_capacity <- 50 parameter:true;
	
	
	init{	
		create road from: split_lines(union(environment collect each.shape.contour));	
		road_network <- as_edge_graph(road);
		
		//initialize with empty buildings everywhere
		loop i from:0 to:7 {
			loop j from:0 to:7 {
				let ev <- environment[i,j];
				empty_building b;
				
				create empty_building {
					location <- ev.location;
					shape <- ev.shape;
					b <- self;
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
		let buildings <- [];
		let people <- [];
		loop b over:empty_building {
			buildings <+ b.to_json();
		}
		loop b over:office {
			buildings <+ b.to_json();
		}
		loop b over:house{
			buildings <+ b.to_json();
		}
		loop p over:inhabitant {
			people <+ p.to_json();
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
	    		if answer["type"] = "house" and house(int(answer["id"])) != nil {
	    			do change_house_to_office(house(int(answer["id"])));
	    		}
	    		else if answer["type"] = "office" and office(int(answer["id"])) != nil {
	    			do change_office_to_empty_building(office(int(answer["id"])));
	    		}
	    		else if answer["type"] = "empty" and empty_building(int(answer["id"])) != nil {
	    			do change_empty_building_to_house(empty_building(int(answer["id"])));
	    		}
	    	}
	    }	
	}

	//when a road is busy, it turns into yellow & inhabitant's speed decreases
	reflex update_road{
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
	}

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



species inhabitant skills:[moving]{
	point office_location <- nil;
	point house_location <- nil;
	point target <- nil;
	rgb color;
	float initial_speed <- 30#km/#h;
	
	init {
		color <- rnd_color(225);
		speed <- initial_speed;
	}

	aspect default{
		draw circle(1) color: color;
	}

	reflex choose_target{		
		if(current_date.hour >= 7 and current_date.hour <= 17){
			target <- office_location;
		}
		else{
			target <- house_location;			
		}
	}
	
	reflex moving {
		if  target = nil {
			target <- any_location_in(world);
		}
		do goto target:target on:road_network speed:speed;			
	}	
	
	map to_json {
		return map("id"::int(self), "location":: map('x'::location.x, 'y'::location.y), "heading"::heading);
	}
}

species road{
	rgb color <- #black;
	aspect default{
		draw shape color: color;
	}
}

species building {
	string type <- "default";
	map to_json {
		return map("id":: int(self), "type":: type, "location":: map('x'::location.x, 'y'::location.y));
	}
}

species empty_building parent:building{
	
	rgb color <- #grey;
	string type <- "empty";
	

}

species house parent:building{
	rgb color <- #blue;
	string type <- "house";
}

species office parent:building{
	rgb color <- #orange;
	string type <- "office";
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
			species road aspect: default;
			species house;
			species office;
			species inhabitant aspect: default;
			event mouse_down action:mouse_click;
		}	
	}
}


