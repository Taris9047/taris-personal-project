//
//  AppDelegate.h
//  My First Application
//
//  Created by Taris on 2014-08-14.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class Track;

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (weak) IBOutlet NSTextField *textField;

@property (weak) IBOutlet NSSlider *slider;

@property (strong) Track *track;

- (IBAction)mute:(id)sender;

- (IBAction)takeFloatValueForVolumeFrom:(id)sender;

- (void)updateUserInterface;


@end
