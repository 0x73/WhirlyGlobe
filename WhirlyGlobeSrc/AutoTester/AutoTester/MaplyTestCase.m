//
//  MaplyTestCase.m
//  AutoTester
//
//  Created by jmWork on 13/10/15.
//  Copyright © 2015 mousebird consulting. All rights reserved.
//

#import "MaplyTestCase.h"

@implementation MaplyTestCase

- (void)runTest
{
	if (self.resultBlock) {
		self.resultBlock(NO);
	}
}

@end
