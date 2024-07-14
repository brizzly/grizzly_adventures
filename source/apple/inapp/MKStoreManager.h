//
//  StoreManager.h
//  MKSync
//
//  Created by Mugunth Kumar on 17-Oct-09.
//  Copyright 2009 MK Inc. All rights reserved.
//  mugunthkumar.com

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "MKStoreObserver.h"
#import "MKStore_interface.h"

@interface MKStoreManager : NSObject<SKProductsRequestDelegate> 
{
	NSMutableArray *purchasableObjects;
	MKStoreObserver *storeObserver;
	BOOL	_lastTransactionFailed;
}

@property (nonatomic, retain) NSMutableArray *purchasableObjects;
@property (nonatomic, retain) MKStoreObserver *storeObserver;

- (int) doSomethingWith :(int)action :(int)parameter;
- (char*) doSomethingWith2:(int)action :(int)parameter;

- (void) requestProductData;

- (void) buyPack1;
- (void) buyPack2;
//- (void) buyPack3;
//- (void) buyPack4;
//- (void) buyPack5;


// do not call this directly. This is like a private method
- (void) buyFeature:(NSString*) featureId;

- (SKProduct*) findProductById:(NSString*)productId;
- (void) failedTransaction: (SKPaymentTransaction *)transaction;
- (BOOL) getFailedTransaction;
- (void) clearProvidedContent:(NSString*) productIdentifier;
//- (int) getProvidedContent:(NSString*) productIdentifier;
- (void) provideContent: (NSString*) productIdentifier;
- (NSString*) getProductCost:(NSString*)productId;



+ (MKStoreManager*) sharedManager;

+ (BOOL) pack1Purchased;
+ (BOOL) pack2Purchased;
+ (BOOL) pack3Purchased;
+ (BOOL) pack4Purchased;
+ (BOOL) pack5Purchased;

+ (void) loadPurchases;
+ (void) updatePurchases;


@end
