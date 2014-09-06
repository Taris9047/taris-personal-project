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

@property double dim_left;
@property double dim_right;
@property double dim_top;
@property double dim_bottom;

@property double vel_max_x;
@property double vel_max_y;

@property double calc_time;
@property double unit_time;
@property unsigned int RNG_type;

@property Molecule* current_object;
@property Physics* Rect_estimation;

@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSButton *ExitButton;

@end
