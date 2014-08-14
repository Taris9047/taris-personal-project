//
//  Car.h
//  Class_Tutorial
//
//  Created by Taris on 2014-08-14.
//  Copyright (c) 2014 Taylor Shin. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Car : NSObject {
    
}

@property (copy) NSString *model;

- (void)drive;

+ (void)setDefaultModel:(NSString *)aModel;

+ (void)readDefaultModel:(NSString *)aModel;

@end
