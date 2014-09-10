//
//  AppDelegate.m
//  BrownianCocoa
//
//  Created by Taris on 2014-08-29.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import "AppDelegate.h"
#import "Physics.hpp"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    _dim_left = -1000.;
    _dim_right = 1000.;
    _dim_top = 1000.;
    _dim_bottom = -1000.;
    
    _vel_max_x = 1000/3;
    _vel_max_y = 1000/3;
    
    _calc_time = 100.;
    _unit_time = 1.;
    _RNG_type = 1;
    
    
}


- (IBAction)OnExitBtnClicked:(id)sender {
    [NSApp terminate: nil];
}
- (IBAction)OnRunBtnClicked:(id)sender {
    // Gotta set these stuff with settings window later.
    
    NSString* brownian_log;
    
    _current_object = new Molecule();
    _Rect_estimation = new Physics(_current_object, _calc_time, _unit_time, true, _RNG_type);
    _Rect_estimation->set_dimension_rect(_dim_left, _dim_right, _dim_top, _dim_bottom);
    _Rect_estimation->brownian_rect(_vel_max_x, _vel_max_y);
    
    brownian_log = [[NSString alloc] initWithUTF8String:_Rect_estimation->sprint_status_rect("\n").c_str()];
    
}


@end
