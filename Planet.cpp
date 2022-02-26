// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
//
//  Planet.cpp
//  p4-heap
//
//  Created by Luciana Solorzano  on 2/9/22.
//

#include "Planet.hpp"
#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include "deployment.hpp"
#include "General.hpp"


void Planet::ambush_sith(deployment& deployment_in){
    if(state_ambush == State::Initial){
        state_ambush = State::SeenOne;
        best_ambush_sith.timestamp = deployment_in.timestamp;
        best_ambush_sith.force_strength = deployment_in.force_strength;
    }
    else if(state_ambush == State::SeenOne && deployment_in.force_strength > best_ambush_sith.force_strength){
        best_ambush_sith.timestamp = deployment_in.timestamp;
        best_ambush_sith.force_strength = deployment_in.force_strength;
    }
    else if(state_ambush == State::SeenBoth && deployment_in.force_strength > best_ambush_sith.force_strength){
        state_ambush = State::MaybeBetter;
        maybe_best_ambush_sith.timestamp = deployment_in.timestamp;
        maybe_best_ambush_sith.force_strength = deployment_in.force_strength;
    }
}

void Planet::attack_sith(deployment& deployment_in){
    if(state_attack == State::SeenOne && best_attack_jedi.force_strength <= deployment_in.force_strength){
        state_attack = State::SeenBoth;
        best_attack_sith.timestamp = deployment_in.timestamp;
        best_attack_sith.force_strength = deployment_in.force_strength;
    }
    else if(state_attack == State::SeenBoth && best_attack_sith.force_strength < deployment_in.force_strength){
        best_attack_sith.timestamp = deployment_in.timestamp;
        best_attack_sith.force_strength = deployment_in.force_strength;
    }
    else if(state_attack == State::MaybeBetter && deployment_in.force_strength >= maybe_best_attack_jedi.force_strength){
        if(deployment_in.force_strength == best_attack_sith.force_strength && maybe_best_attack_jedi.force_strength == best_attack_jedi.force_strength && deployment_in.timestamp < best_attack_sith.timestamp){
            state_ambush= State::SeenBoth;
            return;
        }
        else if(deployment_in.force_strength == best_attack_sith.force_strength && maybe_best_attack_jedi.force_strength == best_attack_jedi.force_strength && maybe_best_attack_jedi.timestamp > best_attack_jedi.timestamp && deployment_in.timestamp == best_attack_sith.timestamp){
            state_ambush= State::SeenBoth;
            return;
        }
        state_attack= State::SeenBoth;
        best_attack_jedi.timestamp = maybe_best_attack_jedi.timestamp;
        best_attack_jedi.force_strength = maybe_best_attack_jedi.force_strength;
        best_attack_sith.timestamp = deployment_in.timestamp;
        best_attack_sith.force_strength = deployment_in.force_strength;
    }
}

void Planet::ambush_jedi(deployment& deployment_in){
    if(state_ambush == State::SeenOne && best_ambush_sith.force_strength >= deployment_in.force_strength){
        state_ambush = State::SeenBoth;
        best_ambush_jedi.timestamp = deployment_in.timestamp;
        best_ambush_jedi.force_strength = deployment_in.force_strength;
    }
    else if(state_ambush == State::SeenBoth && deployment_in.force_strength < best_ambush_jedi.force_strength){
        best_ambush_jedi.timestamp = deployment_in.timestamp;
        best_ambush_jedi.force_strength = deployment_in.force_strength;
    }
    else if(state_ambush == State::MaybeBetter && maybe_best_ambush_sith.force_strength >= deployment_in.force_strength){
        if(deployment_in.force_strength == best_ambush_jedi.force_strength && maybe_best_ambush_sith.force_strength == best_ambush_sith.force_strength && deployment_in.timestamp < best_ambush_jedi.timestamp){
            state_ambush= State::SeenBoth;
            return;
        }
        else if(deployment_in.force_strength == best_ambush_jedi.force_strength && maybe_best_ambush_sith.force_strength == best_ambush_sith.force_strength && maybe_best_ambush_sith.timestamp > best_ambush_sith.timestamp && deployment_in.timestamp == best_ambush_sith.timestamp){
            state_ambush= State::SeenBoth;
            return;
        }
        
        state_ambush= State::SeenBoth;
        best_ambush_sith.timestamp = maybe_best_ambush_sith.timestamp;
        best_ambush_sith.force_strength = maybe_best_ambush_sith.force_strength;
        best_ambush_jedi.timestamp = deployment_in.timestamp;
        best_ambush_jedi.force_strength = deployment_in.force_strength;
    }
}

void Planet::attack_jedi(deployment& deployment_in){
    if(state_attack == State::Initial){
        state_attack = State::SeenOne;
        best_attack_jedi.timestamp = deployment_in.timestamp;
        best_attack_jedi.force_strength = deployment_in.force_strength;
    }
    else if(state_attack == State::SeenOne && deployment_in.force_strength < best_attack_jedi.force_strength){
        best_attack_jedi.timestamp = deployment_in.timestamp;
        best_attack_jedi.force_strength = deployment_in.force_strength;
    }
    else if(state_attack == State::SeenBoth && deployment_in.force_strength < best_attack_jedi.force_strength){
        state_attack = State::MaybeBetter;
        maybe_best_attack_jedi.timestamp = deployment_in.timestamp;
        maybe_best_attack_jedi.force_strength = deployment_in.force_strength;
    }
}

void Planet::add_deployment(bool watch, deployment& deployment_in, std::string& in){
    if(in == "SITH"){
        deployments_sith.push(deployment_in);
        if(watch){
            ambush_sith(deployment_in);
            attack_sith(deployment_in);
        }
        
    }
    else{
        deployments_jedi.push(deployment_in);
        if(watch){
            attack_jedi(deployment_in);
            ambush_jedi(deployment_in);
        }
    }
}

bool Planet::is_opposing_empty(std::string s_in){
    if (s_in == "SITH"){
        return deployments_jedi.empty();
    }
    else {
        return deployments_sith.empty();
    }
}

bool Planet::is_current_empty(std::string s_in){
    if (s_in == "SITH"){
        return deployments_sith.empty();
    }
    else {
        return deployments_jedi.empty();
    }
}


bool Planet::do_war(bool verb_in, bool is_gen, bool median, std::vector<General>& g_in, int& num_b){
    uint32_t troops_lost = 0;
    uint32_t gen_n_1 = 0;
    uint32_t gen_n_2 = 0;
    
    
    if(deployments_sith.empty()){
        return false;
    }
    if(deployments_jedi.empty()){
        return false;
    }
    
    if(deployments_jedi.top().force_strength > deployments_sith.top().force_strength){
        return false;
    }

    
    if(deployments_sith.top().num_troops < deployments_jedi.top().num_troops){
        //update general vector and decrease total amount of troops
        if(is_gen){
            g_in[deployments_jedi.top().gen_num].total_troops -= deployments_sith.top().num_troops;
            //update this general's total numbe rof troops
            g_in[deployments_sith.top().gen_num].total_troops -= deployments_sith.top().num_troops;
        }
        //update top jedi element so that all the troops deleted from the other deployment will be
        //subtracted from that generals total num troops
        deployments_jedi.top().num_troops -= deployments_sith.top().num_troops;
        //set sith troops equal to zero and then pop from pq so that the next deployment can get a chance to fight
        troops_lost = deployments_sith.top().num_troops * 2;
        gen_n_1 = deployments_jedi.top().gen_num;
        gen_n_2 = deployments_sith.top().gen_num;
        deployments_sith.top().num_troops = 0;
        deployments_sith.pop();
    }
    else if(deployments_sith.top().num_troops == deployments_jedi.top().num_troops){
        //
        if(is_gen){
            g_in[deployments_jedi.top().gen_num].total_troops -= deployments_jedi.top().num_troops;
            g_in[deployments_sith.top().gen_num].total_troops -= deployments_sith.top().num_troops;
        }
        troops_lost = deployments_sith.top().num_troops * 2;
        gen_n_1 = deployments_jedi.top().gen_num;
        gen_n_2 = deployments_sith.top().gen_num;
        //if the number of troops for opposing teams are equal, then pop off both and change the generals vector
        deployments_jedi.pop();
        deployments_sith.pop();
        
    }
    else if(deployments_sith.top().num_troops > deployments_jedi.top().num_troops){
        if(is_gen){
            g_in[deployments_sith.top().gen_num].total_troops -= deployments_jedi.top().num_troops;
            g_in[deployments_jedi.top().gen_num].total_troops -= deployments_jedi.top().num_troops;
        }
        deployments_sith.top().num_troops -= deployments_jedi.top().num_troops;
        troops_lost = deployments_jedi.top().num_troops * 2;
        gen_n_2 = deployments_sith.top().gen_num;
        gen_n_1 = deployments_jedi.top().gen_num;
        deployments_jedi.top().num_troops = 0;
        deployments_jedi.pop();
    }
    
    if(verb_in && (troops_lost > 0)){
        //print verbose output
        std::cout << "General "<< gen_n_2 << "'s battalion attacked General " << gen_n_1 << "'s battalion on planet " << planet_ID << ". " << troops_lost << " troops were lost.\n";
        
    }
    
    //increment the number of battles in galaxy class. needs to be a paramter from the galaxy class and
    //not a member of planet class because there are going to be multiple planets (therefore passed by reference)
    if(troops_lost > 0){
        ++num_b;
    }
    
    
    //this is for the first case where we have to add somehting to the max pq first
    if(median && median_first_half.empty() && troops_lost > 0){
        median_first_half.push(troops_lost);
        return true;
    }
    
    
    //then in the next iteration if the num troops lost is less than what is
    //already in the first half you add that to the first half
    if(median && troops_lost < median_first_half.top() && troops_lost > 0){
        median_first_half.push(troops_lost);
        
        //if the size of the first half is 2 greater after the add then move the top
        //to the min pq second half and it will now be the top element of that pq and then you pop
        //from the first half and the sizes are now equal
        if( median_first_half.size() == (median_second_half.size() + 2)){
            median_second_half.push(median_first_half.top());
            median_first_half.pop();
        }
    }
    
    //conversely if the num troops is greater than the top of the first half
    //then add numtroops to the second half because it is bigger than the first
    else if (median && troops_lost > median_first_half.top() && troops_lost > 0){
        median_second_half.push(troops_lost);
        //if the size of the second half is 2  larger than the first then add to the frist
        //and pop the second and since this value is greater than the top
        if(median_second_half.size() == (median_first_half.size() + 2)){
            median_first_half.push(median_second_half.top());
            median_second_half.pop();
        }
    }
    else if(median && troops_lost == median_first_half.top() && troops_lost > 0){
        median_second_half.push(troops_lost);
        //if the size of the second half is 2  larger than the first then add to the frist
        //and pop the second and since this value is greater than the top
        if(median_second_half.size() == (median_first_half.size() + 2)){
            median_first_half.push(median_second_half.top());
            median_second_half.pop();
        }
    }
    
    //this boolean was made to determine whether to continue in read run loop
    //it is only set to false when the top jedi element os larger than the top sith element
     
     return true;
    
    
}
