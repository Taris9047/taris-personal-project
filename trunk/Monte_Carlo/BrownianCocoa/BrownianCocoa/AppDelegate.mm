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
    
}


- (IBAction)OnExitBtnClicked:(id)sender {
    [NSApp terminate: nil];
}
- (IBAction)OnRunBtnClicked:(id)sender {
    // Gotta set these stuff with settings window later.
    _dim_left = -1000.;
    _dim_right = 1000.;
    _dim_top = 1000.;
    _dim_bottom = -1000.;
    
    _vel_max_x = 500;
    _vel_max_y = -500;
    
    NSString* brownian_log;
    
    Molecule* current_object = new Molecule();
    Physics* Rect_estimation = new Physics(current_object);
    
    Rect_estimation->set_dimension_rect(_dim_left, _dim_right, _dim_top, _dim_bottom);
    Rect_estimation->brownian_rect(_vel_max_x, _vel_max_y);
    
    brownian_log = [[NSString alloc] initWithUTF8String:Rect_estimation->sprint_status_rect("\n").c_str()];
    
    
}


@end
