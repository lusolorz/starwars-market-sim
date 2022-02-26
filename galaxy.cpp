// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

//  galaxy.cpp
//  p4-heap
//
//  Created by Luciana Solorzano  on 2/9/22.
//

#include "galaxy.hpp"
#include "Planet.hpp"
#include "P2random.h"
#include "General.hpp"
//#include "deployment.hpp"
#include <iostream>
#include <cstring>
#include <queue>
#include <sstream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include "xcode_redirect.hpp"

class Galaxy{
    
private:
    bool verbose = false;
    bool median = false;
    bool general = false;
    bool watcher = false;
    bool DL_mode = false;
    bool PR_mode = false;
    
    uint32_t num_planets = 0;
    uint32_t num_generals = 0;
    uint32_t current_timestamp = 0;
    int num_battles = 0;
    
    std::vector<Planet> planets;
    std::vector<General> all_generals;
    


public:
    
    void get_options(int argc, char** argv){
        int option_index = 0, option = 0;
        
        // Don't display getopt error messages about options
        //opterr = false;

        // use getopt to find command line options
        struct option longOpts[] = {
            { "median", no_argument, nullptr, 'm' },
            { "general-eval", no_argument, nullptr, 'g' },
            { "watcher",no_argument , nullptr, 'w'},
            { "verbose", no_argument, nullptr, 'v'},
            { nullptr, 0, nullptr, '\0' }
        };
        

        while ((option = getopt_long(argc, argv, "mgwv", longOpts, &option_index)) != -1) {
            switch (option) {
                case 'm':{
                    median = true;
                    break;
                }
                case 'g':{
                    general = true;
                    break;
                }
                    
                case 'w':{
                    watcher = true;
                    break;
                }
                case 'v':{
                    verbose = true;
                    break;
                }
                default:
                    exit(1);
            }
        }
    }

    void read_run_data(){
        std::string comment = "\0";
        getline(std::cin, comment, '\n');
        getline(std::cin, comment, '\n');
        if(comment[6] == 'D'){
            DL_mode = true;
        }
        else if(comment[6] == 'P'){
            PR_mode = true;
        }
        getline(std::cin, comment, '\n');
        comment.erase(0,13);
        uint32_t hold = uint32_t(stoi(comment));
        num_generals = hold;
        if(general){
            all_generals.resize(num_generals);
        }
        getline(std::cin, comment, '\n');
        comment.erase(0,12);
        hold = (uint32_t)(stoi(comment));
        num_planets = hold;
        std::stringstream ss;
        planets.resize(num_planets);
        
        if(PR_mode){
            unsigned int seed = 0;
            unsigned int num_deployments = 0;
            unsigned int rate = 0;
            getline(std::cin, comment, '\n');
            comment.erase(0,13);
            seed = (unsigned int)(stoi(comment));
            getline(std::cin, comment, '\n');
            comment.erase(0,17);
            num_deployments = (unsigned int) (stoi(comment));
            getline(std::cin, comment, '\n');
            comment.erase(0,14);
            rate = (unsigned int) (stoi(comment));
            P2random::PR_init(ss, seed, num_generals, num_planets, num_deployments, rate);
            //P2random::PR_init(ss, seed, num_generals, num_planets, num_deployments, rate);
        }
        std::istream &inputStream = PR_mode == true ? ss: std::cin;
        std::string timestamp;
        std::string force_side;
        std::string general_in;
        std::string force_strength;
        std::string num_troops;
        std::string planet;
        uint32_t uniq_ID = 0;
        while(inputStream >> timestamp >> force_side >> general_in >> planet >> force_strength >> num_troops){
            uint32_t timestamp_num = (uint32_t)(stoi(timestamp));
            planet.erase(0,1);
            uint32_t planet_num = (uint32_t)(stoi(planet));
            if(planet_num >= num_planets){
                std::cerr << "Invalid planet num";
                exit(1);
            }
            force_strength.erase(0,1);
            
            int32_t force_strength_num_hold = (int32_t)(stoi(force_strength));
            //check to see if force strength is not greater than 0 so less than or equal to
            if(force_strength_num_hold <= 0){
                std::cerr << "Invalid force strength\n";
                exit(1);
            }
            uint32_t force_strength_num = uint32_t(force_strength_num_hold);
            general_in.erase(0,1);
            int32_t general_num_hold = int32_t(stoi(general_in));
            //check if num generals is ggreater than or equal to total number of generals
            //or less than 0
            if(general_num_hold < 0 || general_num_hold >= (int32_t)num_generals){
                std::cerr << "Invalid general\n";
                exit(1);
            }
            uint32_t general_num = uint32_t(general_num_hold);
            num_troops.erase(0,1);
            int32_t num_troops_num_hold = (int32_t)(stoi(num_troops));
            //check to see if number of troops is not greater than 0 so less than or equal to
            if(num_troops_num_hold <= 0){
                std::cerr << "Invalid number of troops\n";
                exit(1);
            }
            uint32_t num_troops_num = uint32_t(num_troops_num_hold);
            if(force_side == "JEDI" && general){
                all_generals[general_num].num_jedi_troops += num_troops_num;
                all_generals[general_num].total_troops += num_troops_num;
            }
            else if (force_side == "SITH" && general) {
                all_generals[general_num].num_sith_troops += num_troops_num;
                all_generals[general_num].total_troops += num_troops_num;
            }
            //timestamps must be increasing
            if(timestamp_num < current_timestamp){
                std::cerr << "Invalid timestamp";
                exit(1);
            }
            if(current_timestamp != timestamp_num && median){
                for(uint32_t i = 0; i < planets.size(); ++i){
                    //if both pq are empty then a battle didn't occur on that planet
                    if(planets[i].median_first_half.empty() && planets[i].median_second_half.empty()){
                        continue;
                    }
                    //if the pqs equal each other find average of tops
                    if(planets[i].median_first_half.size() == planets[i].median_second_half.size()){
                        std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << (planets[i].median_first_half.top() + planets[i].median_second_half.top()) / 2 << ".\n";
                    }
                    //if first half is greater that means the middle element is in the first half
                    else if(planets[i].median_first_half.size() > planets[i].median_second_half.size()){
                        std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << planets[i].median_first_half.top() << ".\n";
                    }
                    //if median  first half is less than the second then the median is in the second half
                    else if (planets[i].median_first_half.size() < planets[i].median_second_half.size()){
                        std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << planets[i].median_second_half.top() << ".\n";
                    }
                }
                        
            }
            
            current_timestamp = timestamp_num;
            
            deployment current(timestamp_num, general_num, planet_num, force_strength_num, num_troops_num, uniq_ID);

            planets[planet_num].add_deployment(watcher, current, force_side);
            //this bool is changed in do war and willl tell you to exit if the jedi pq top
            //has a higher force strength than whatever is in top sith pq
            bool s = true;
            
            while(s){
                //for verbose
                planets[planet_num].set_planet_ID(planet_num);
                s = planets[planet_num].do_war(verbose, general, median, all_generals, num_battles);
                ++uniq_ID;
            }
            
            
        }
        
        for(uint32_t i = 0; i < planets.size() && median; ++i){
            //if both pq are empty then a battle didn't occur on that planet
            if(planets[i].median_first_half.empty() && planets[i].median_second_half.empty() && median){
                    continue;
            }
            if(planets[i].median_first_half.size() == planets[i].median_second_half.size() && median){
                std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << (planets[i].median_first_half.top() + planets[i].median_second_half.top()) / 2 << ".\n";
            }
            else if(planets[i].median_first_half.size() > planets[i].median_second_half.size() && median){
                std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << planets[i].median_first_half.top() << ".\n";
            }
            else if (planets[i].median_first_half.size() < planets[i].median_second_half.size() && median){
                std::cout << "Median troops lost on planet " << i << " at time " << current_timestamp << " is " << planets[i].median_second_half.top() << ".\n";
            }
        }
            
            //movie watcher
                    
        std::cout << "---End of Day---\n";
        std::cout << "Battles: " << num_battles << "\n";
        if(general){
            std::cout << "---General Evaluation---\n";
            for(uint32_t i = 0; i < all_generals.size(); ++i){
                std::cout << "General " << i << " deployed " << all_generals[i].num_jedi_troops << " Jedi troops and " << all_generals[i].num_sith_troops << " Sith troops, and " << all_generals[i].total_troops << "/" << all_generals[i].num_jedi_troops +all_generals[i].num_sith_troops << " troops survived.\n";
            }
        }
        if(watcher){
            std::cout << "---Movie Watcher---\n";
            for(uint32_t i = 0; i < planets.size() && watcher; ++i){
                if(planets[i].best_ambush_jedi.force_strength == 0 || planets[i].best_ambush_sith.force_strength == 0){
                    std::cout << "A movie watcher would enjoy an ambush on planet " << i << " with Sith at time -1 and Jedi at time -1 with a force difference of 0.\n";
                }
                else if(planets[i].best_ambush_jedi.force_strength > 0 && planets[i].best_ambush_sith.force_strength > 0){
                    std::cout << "A movie watcher would enjoy an ambush on planet " << i << " with Sith at time " << planets[i].best_ambush_sith.timestamp << " and Jedi at time " << planets[i].best_ambush_jedi.timestamp << " with a force difference of " << planets[i].best_ambush_sith.force_strength -
                        planets[i].best_ambush_jedi.force_strength << ".\n";
                }
                if(planets[i].best_attack_jedi.force_strength == 0 || planets[i].best_attack_sith.force_strength == 0){
                    std::cout << "A movie watcher would enjoy an attack on planet " << i << " with Jedi at time -1 and Sith at time -1 with a force difference of 0.\n";
                }
                else if(planets[i].best_attack_jedi.force_strength > 0 || planets[i].best_attack_sith.force_strength > 0){
                    std::cout << "A movie watcher would enjoy an attack on planet " << i << " with Jedi at time " << planets[i].best_attack_jedi.timestamp << " and Sith at time " << planets[i].best_attack_sith.timestamp << " with a force difference of " << planets[i].best_attack_sith.force_strength-
                        planets[i].best_attack_jedi.force_strength << ".\n";
                }
            }
        }
    }
};



int main(int argc, char *argv[]){
    // read input
    
    std::ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    
    Galaxy far_far_away;
    
    far_far_away.get_options(argc, argv);
    
    std::cout << "Deploying troops...\n";
     
    far_far_away.read_run_data();

    
}
