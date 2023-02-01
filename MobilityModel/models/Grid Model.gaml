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
	float last_duration <- 0;
	bool initialized <- false;
	
	
	// Simulation parameters
	float step <- 1#second;
	
	list available_office <- [];
	graph road_network;
	map<road,float> new_weights;
	geometry shape <- square(30*8);
	
	init{	
		create road from: split_lines(union(environment collect each.shape.contour));	
		road_network <- as_edge_graph(road);
		//do send_init_data;
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

	reflex update_ue 
	{
		
		if ! initialized {
			do send_init_data;
			initialized <- true;
		}
		else {
			do send_world;			
		}
		last_duration <- total_duration;
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
					office_location <- not empty(available_office) ? any_location_in(one_of(available_office))  : house_location;					
				}										
			}
		}
	}

	reflex update_available_office{
		loop i over: office{
			if(length(inhabitant overlapping i) < 20 and !(i in available_office)){
				available_office <- available_office + i;
			}
			if((i in available_office) and (length(inhabitant overlapping i) >= 20)){
				available_office <- available_office - i;
			}	
		}
	}
	
	reflex send_update_data {
		
	}
	
	action mouse_click{
		environment selected_cell <- first(environment overlapping (#user_location));
		//write selected_cell;
		if selected_cell != nil{
			if(house overlapping (#user_location) = [] and office overlapping (#user_location) = []){
				create house{
				location <- selected_cell.location;
				color <- #blue;
				shape <- selected_cell.shape;
				create inhabitant number: 20{
						location <- any_location_in((selected_cell).shape);
						house_location <- location;
						office_location <- not empty(available_office) ? any_location_in(one_of(available_office)) : nil;
					}				
				}
			}
			
			else if(house overlapping (#user_location) != []){
				ask house overlapping (#user_location) {
					//kill inhabitant belonging to that house
					loop i over: inhabitant{
						if (selected_cell overlaps i.house_location){
							ask i{
								do die;
							}
						}				
					}
					create office{
						color <- #orange;
						location <- myself.location;
						shape <- myself.shape;	
					}		
				do die;
				}
			}
			
			else {
				ask office overlapping (#user_location){
					ask inhabitant inside(self){
						location <- not empty(available_office) ? any_location_in(one_of(available_office)) : house_location;
					}
					available_office >> self;
					do die;
				}			
//				write ('number of available office after kill one: ' + length(available_office));
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
	
	reflex moving when: target != nil{
		do goto target:target on:road_network speed:speed;
		if (location = target){
			target <-  nil;
		}
	}	
	
	map to_json {
		return map("id"::int(self), "location":: map('x'::location.x, 'y'::location.y));
	}
}

species road{
	rgb color <- #black;
	aspect default{
		draw shape color: color;
	}
}

species empty_building{
	rgb color;
	string type <- "empty";
	aspect default{
		draw shape color:color;
	}
	
	map to_json {
		return map("id":: int(self), "type":: type, "location":: map('x'::location.x, 'y'::location.y));
	}
}

species house parent:empty_building{
	string type <- "house";
}

species office parent:empty_building{
	string type <- "office";
}

grid environment height:8 width:8 neighbors:4{
	list<environment> neighbors2 <- self neighbors_at 2;

}

experiment grid_model type:gui{

	float minimum_cycle_duration <- 0.1#second;

	output{
		
		display main_display type:opengl axes:false{
			//grid environment border: #black;
			species empty_building aspect: default;
			species road aspect: default;
			species house aspect: default;
			species office aspect: default;
			species inhabitant aspect: default;
			event mouse_down action:mouse_click;
		}	
	}
}
