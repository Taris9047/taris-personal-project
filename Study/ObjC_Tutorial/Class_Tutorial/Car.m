//
//  Car.m
//  Class_Tutorial
//
//  Created by Taris on 2014-08-14.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import "Car.h"

static NSString *_defaultModel;

@implementation Car {
    double _odometer;
}

@synthesize model = _model;

- (void)drive {
    NSLog(@"Driving a %@. Vroom!", self.model);
}

+ (void)setDefaultModel:(NSString *)aModel {
    _defaultModel = [aModel copy];
}

+ (void)readDefaultModel:(NSString *)aModel {
    aModel = [_defaultModel copy];
}

@end
