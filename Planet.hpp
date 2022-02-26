// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
//
//  Planet.hpp
//
//  p4-heap
//
//  Created by Luciana Solorzano  on 2/9/22.
//

#ifndef Planet_hpp
#define Planet_hpp

# include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include "deployment.hpp"
#include "General.hpp"
#include "small_deploy.hpp"

#endif /* Planet_hpp */

enum class State{
    Initial,
    SeenOne,
    SeenBoth,
    MaybeBetter
};


class Jedi_compare{
public:
    bool operator () (const deployment& left, const deployment& right){
        if(left.force_strength == right.force_strength){
            return(left.unique_ID > right.unique_ID);
        }
        return (left.force_strength > right.force_strength);
    }
};

class Sith_compare{
public:
    bool operator () (const deployment& left, const deployment& right){
        if(left.force_strength == right.force_strength){
            return(left.unique_ID > right.unique_ID);
        }
        return (left.force_strength < right.force_strength);
    }
};

class Planet{
public:
    
    //adds deployment to either the sith pq or jedi pq
    void ambush_sith(deployment& deployment_in);
    void attack_sith(deployment& deployment_in);
    void ambush_jedi(deployment& deployment_in);
    void attack_jedi(deployment& deployment_in);
    
    void add_deployment(bool watch, deployment& deployment_in, std::string& in);
    
    bool is_opposing_empty(std::string s_in);
    
    bool is_current_empty(std::string s_in);
    
    deployment return_top(){
        return deployments_jedi.top();
    }
    
    
    //if opposing is empty equals false then do war. 
    
    
    //determines which deployment to return based on ordering of sith and jedi
    deployment priority();
    
    void set_planet_ID(uint32_t planet_num){
        planet_ID = planet_num;
    }
    
    //compares the two top pq elements and returns a functor which would be a call to priority
    //maybe should include a conditional to see if opposite pq is empty
    bool do_war(bool verb_in, bool is_gen, bool median , std::vector<General>& g_in, int& num_b);
    uint32_t update_total_troops_general(uint32_t general);
    
    
private:
    
    uint32_t planet_ID = 0;
    std::priority_queue<deployment, std::vector<deployment>, Jedi_compare> deployments_jedi;
    std::priority_queue<deployment, std::vector<deployment>, Sith_compare> deployments_sith;
    
public:
    //we want the smallest item of min heap to be at the top of its pq
    std::priority_queue< unsigned int, std::vector<unsigned int>, std::greater<unsigned int> > median_second_half;
    //want the largest of the first half to be on top to get the mdeian between the two
    std::priority_queue<unsigned int> median_first_half;
    //movie watcher state machine
    
    State state_attack = State::Initial;
    State state_ambush = State::Initial;
    
    smaller_deploy best_ambush_jedi;
    smaller_deploy best_ambush_sith;
    smaller_deploy best_attack_jedi;
    smaller_deploy best_attack_sith;
    //smaller_deploy maybe_best_ambush_jedi;
    smaller_deploy maybe_best_ambush_sith;
    smaller_deploy maybe_best_attack_jedi;
    //smaller_deploy maybe_best_attack_sith;
    
};
