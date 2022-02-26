// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
//  small_deploy.hpp
//  p4-heap
//
//  Created by Luciana Solorzano  on 2/19/22.
//

#ifndef small_deploy_hpp
#define small_deploy_hpp

#include <stdio.h>

struct smaller_deploy{
    uint32_t timestamp = 0;
    uint32_t force_strength = 0;

    smaller_deploy(uint32_t time, uint32_t force_s): timestamp(time),force_strength(force_s){}
    
    smaller_deploy() = default;
    
};

#endif /* small_deploy_hpp */
