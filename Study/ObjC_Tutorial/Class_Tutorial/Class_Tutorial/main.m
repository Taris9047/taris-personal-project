//
//  main.m
//  Class_Tutorial
//
//  Created by Taris on 2014-08-14.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Car.h"

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        [Car setDefaultModel:@"Nissan Versa"];
        
        NSString* defaultModel_Car;
        [Car readDefaultModel:defaultModel_Car];
        NSLog(defaultModel_Car);
        
        Car *toyota = [[Car alloc] init];
  
        //NSLog(@"Default Model %@", [toyota model]);
        
        [toyota setModel:@"Toyota Corolla"];
        NSLog(@"Created a %@", [toyota model]);
        
        toyota.model = @"toyota Camry";
        NSLog(@"Changed the car to a %@", toyota.model);
        
        [toyota drive];
        
    }
    return 0;
}

