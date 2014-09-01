//
//  AppDelegate.h
//  BrownianCocoa
//
//  Created by Taris on 2014-08-29.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property double dim_left;
@property double dim_right;
@property double dim_top;
@property double dim_bottom;

@property double vel_max_x;
@property double vel_max_y;


@property (assign) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSButton *ExitButton;

@end
