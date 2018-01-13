//
//  AppDelegate.h
//  BrownianCocoa
//
//  Created by Taris on 2014-08-29.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Physics.hpp"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) double dim_left;
@property (assign) double dim_right;
@property (assign) double dim_top;
@property (assign) double dim_bottom;

@property (assign) double vel_max_x;
@property (assign) double vel_max_y;

@property (assign) double calc_time;
@property (assign) double unit_time;
@property (assign) unsigned int RNG_type;

@property (assign) Molecule* current_object;
@property (assign) Physics* Rect_estimation;

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSButton *ExitButton;

@end
