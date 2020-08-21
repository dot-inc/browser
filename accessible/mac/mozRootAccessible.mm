/* -*- Mode: Objective-C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "RootAccessibleWrap.h"

#import "mozRootAccessible.h"

#import "mozView.h"

// This must be included last:
#include "nsObjCExceptions.h"

using namespace mozilla::a11y;

static id<mozAccessible, mozView> getNativeViewFromRootAccessible(Accessible* aAccessible) {
  RootAccessibleWrap* root = static_cast<RootAccessibleWrap*>(aAccessible->AsRoot());
  id<mozAccessible, mozView> nativeView = nil;
  root->GetNativeWidget((void**)&nativeView);
  return nativeView;
}

#pragma mark -

@implementation mozRootAccessible

- (id)initWithAccessible:(mozilla::a11y::AccessibleOrProxy)aAccOrProxy {
  NS_OBJC_BEGIN_TRY_ABORT_BLOCK_NIL;

  MOZ_ASSERT(!aAccOrProxy.IsProxy(), "mozRootAccessible is never a proxy");

  mParallelView = getNativeViewFromRootAccessible(aAccOrProxy.AsAccessible());

  return [super initWithAccessible:aAccOrProxy];

  NS_OBJC_END_TRY_ABORT_BLOCK_NIL;
}

- (NSNumber*)moxMain {
  return @([[self moxWindow] isMainWindow]);
}

- (NSNumber*)moxMinimized {
  return @([[self moxWindow] isMiniaturized]);
}

// return the AXParent that our parallell NSView tells us about.
- (id)moxUnignoredParent {
  NS_OBJC_BEGIN_TRY_ABORT_BLOCK_NIL;

  if (!mParallelView) mParallelView = (id<mozView, mozAccessible>)[self representedView];

  if (mParallelView)
    return [mParallelView accessibilityAttributeValue:NSAccessibilityParentAttribute];

  MOZ_ASSERT(mParallelView, "we're a root accessible w/o native view?");
  return [super moxUnignoredParent];

  NS_OBJC_END_TRY_ABORT_BLOCK_NIL;
}

- (BOOL)hasRepresentedView {
  return YES;
}

// this will return our parallell NSView. see mozDocAccessible.h
- (id)representedView {
  NS_OBJC_BEGIN_TRY_ABORT_BLOCK_NIL;

  MOZ_ASSERT(mParallelView, "root accessible does not have a native parallel view.");

  return mParallelView;

  NS_OBJC_END_TRY_ABORT_BLOCK_NIL;
}

- (BOOL)isRoot {
  return YES;
}

@end