//
//  main.m
//  C_Basics
//
//  Created by Taris on 2014-08-14.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Foundation/Foundation.h>

#define PI 3.141592
#define RAD_TO_DEG(radians) (radians * 180 / PI)

typedef unsigned char ColorComponent;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color;

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        // Dealing with stdout
        double odometer = 9200.8;
        int odometerAsInteger = (int)odometer;
        
        NSLog(@"You've driven %.1f miles.", odometer);
        NSLog(@"You've driven %d miles.", odometerAsInteger);
        
        double const pi = 3.141592;
        //pi = 323; // don't do this on const.
        
        // Arithmetic
        NSLog(@"6 + 2 = %d", 6+2);
        NSLog(@"6 %% 5 = %d", 6%5);
        int i = 0;
        NSLog(@"%d", i);
        i++;
        NSLog(@"%d", i);
        i++;
        NSLog(@"%d", i);
        
        // Loops
        int modelYear = 1990;
        
        i = 0;
        while (i<5) {
            if (i == 3) {
                NSLog(@"Aborting the while-loop");
                break;
            }
            NSLog(@"Current year: %d", modelYear+i);
            i++;
        }
        
        for (i=0; i<5; i++) {
            if (i == 3) {
                NSLog(@"Skipping a for-loop iteration.");
                continue;
            }
            NSLog(@"Current year: %d", modelYear+i);
        }
        
        // Macros
        double angle = PI / 2;
        NSLog(@"%f", RAD_TO_DEG(angle));
        
        // Typedef
        ColorComponent red = 255;
        ColorComponent green = 160;
        ColorComponent blue = 0;
        NSLog(@"Your paint job is (R: %hhu, G: %hhu, B: %hhu", red, green, blue);
        
        // Structs
        Color carColor = {255, 160, 0};
        NSLog(@"Your paint job is (R: %hhu, G: %hhu, B: %hhu", \
              carColor.red, carColor.green, carColor.blue);
        
        // Primitive Arrays
        int years[4] = {1968, 1970, 1989, 1999};
        years[0] = 1967;
        for (i=0; i<4; i++) {
            NSLog(@"The year at index %d is: %d", i, years[i]);
        }
        
        // Pointers
        int year = 1967;
        int *year_pointer;
        year_pointer = &year;
        NSLog(@"%d", *year_pointer);
        *year_pointer = 1990;
        NSLog(@"%d", year);
        
        char model[5] = {'H', 'o', 'n', 'd', 'a'};
        char *modelPointer = &model[0];
        for (i=0; i<5; i++) {
            NSLog(@"Value at memory address %p is %c", \
                  modelPointer, *modelPointer);
            modelPointer++;
        }
        NSLog(@"The first letter is %c", *(modelPointer-5));
        
        int *year_n_pointer = &year;
        NSLog(@"%d", *year_n_pointer);
        year_n_pointer = NULL;
        
        void *genericPointer = &year;
        int *intPointer = (int *)genericPointer;
        NSLog(@"%d", *intPointer);
        
        NSString *ns_str_model = @"Honda";
        NSLog(ns_str_model);
        
        NSString *anObject;
        anObject = NULL;
        anObject = nil;
        int *aPointer;
        aPointer = nil;
        aPointer = NULL;
        
    }
    return 0;
}

