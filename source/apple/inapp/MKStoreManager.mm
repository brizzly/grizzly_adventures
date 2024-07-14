//
//  MKStoreManager.m
//
//  Created by Mugunth Kumar on 17-Oct-09.
//  Copyright 2009 Mugunth Kumar. All rights reserved.
//  mugunthkumar.com
//
// andro2@jadetest.com Jadetest2
//


#import "MKStoreManager.h"

@implementation MKStoreManager


@synthesize purchasableObjects;
@synthesize storeObserver;

static NSString * pack1 = @"com.grizzlymac.greenhill";
static NSString * pack2 = @"com.grizzlymac.lostmine";
//static NSString * pack3 = @"com.jadegame.andro2mac.amazone";
//static NSString * pack4 = @"com.jadegame.andro2mac.circus";
//static NSString * pack5 = @"com.jadegame.andro2mac.vegas";

BOOL pack1Purchased = NO;
BOOL pack2Purchased = NO;
BOOL pack3Purchased = NO;
BOOL pack4Purchased = NO;
BOOL pack5Purchased = NO;


static MKStoreManager* _sharedStoreManager;


int MKStoreAction (int action, int parameter)
{
    return [[MKStoreManager sharedManager] doSomethingWith:action :parameter];
}

char * MKStoreAction2 (int action, int parameter)
{
    return [[MKStoreManager sharedManager] doSomethingWith2:action:parameter];
}

- (char*) doSomethingWith2:(int)action :(int)parameter
{
	switch (action)
	{
		case MK_ACTION2_GETPRODUCTPRICE:
		{
			if(parameter == 10) {
				NSString * cost = [self getProductCost:pack1];
				return (char*)[cost UTF8String];
			}
			if(parameter == 20) {
				NSString * cost = [self getProductCost:pack2];
				return (char*)[cost UTF8String];
			}
			/*if(parameter == 30) {
				NSString * cost = [self getProductCost:pack3];
				return (char*)[cost UTF8String];
			}
			if(parameter == 40) {
				NSString * cost = [self getProductCost:pack4];
				return (char*)[cost UTF8String];
			}
			if(parameter == 50) {
				NSString * cost = [self getProductCost:pack5];
				return (char*)[cost UTF8String];
			}*/
		}
		break;
	}
	return nil;
}

- (int) doSomethingWith:(int)action :(int)parameter
{
	switch (action)
	{
		case MK_ACTION_LOAD:
		{
			[MKStoreManager sharedManager];
			
//			pack1Purchased = NO;
//			pack2Purchased = NO;
//			pack3Purchased = NO;
//			pack4Purchased = NO;
//			pack5Purchased = NO;
//			[MKStoreManager updatePurchases];
		}
		break;
			
		case MK_ACTION_CLEARPROVIDEDCONTENT:
		{
		}
		break;
			
		case MK_ACTION_PROVIDEDCONTENT:
		{
			return 0;
		}
		break;
			
		case MK_ACTION_GETFAILEDTRANS:
		{
			return [self getFailedTransaction] == YES ? 1 : 0;
		}
		break;
			
		case MK_ACTION_RESTORE:
		{
			dispatch_async(dispatch_get_main_queue(), ^{
				[self restoreCompletedTransactions];
			});
		}
		break;
			
		case MK_ACTION_ISBOUGHT_PACK1:
		{
			return pack1Purchased ? 1 : 0;
		}
		break;
			
		case MK_ACTION_ISBOUGHT_PACK2:
		{
			//return 1; //
			return pack2Purchased ? 1 : 0;
		}
		break;
			/*
		case MK_ACTION_ISBOUGHT_PACK3:
		{
			//return 1; //
			return pack3Purchased ? 1 : 0;
		}
		break;
			
		case MK_ACTION_ISBOUGHT_PACK4:
		{
			//return 1; //
			return pack4Purchased ? 1 : 0;
		}
		break;
			
		case MK_ACTION_ISBOUGHT_PACK5:
		{
			//return 1; //
			return pack5Purchased ? 1 : 0;
		}
		break;
			*/
		case MK_ACTION_BUY_PACK1:
		{
			dispatch_async(dispatch_get_main_queue(), ^{

				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ON_EXIT_FULLSCREEN object:nil];
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTOP_ANIM object:nil];
				[[MKStoreManager sharedManager] buyFeature:pack1];
			});
		}
		break;
			
		case MK_ACTION_BUY_PACK2:
		{
			dispatch_async(dispatch_get_main_queue(), ^{
		
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ON_EXIT_FULLSCREEN object:nil];
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTOP_ANIM object:nil];
				[[MKStoreManager sharedManager] buyFeature:pack2];
			});
		}
		break;
			/*
		case MK_ACTION_BUY_PACK3:
		{
			dispatch_async(dispatch_get_main_queue(), ^{
				
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ON_EXIT_FULLSCREEN object:nil];
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTOP_ANIM object:nil];
				[[MKStoreManager sharedManager] buyFeature:pack3];
			});
		}
		break;
			
		case MK_ACTION_BUY_PACK4:
		{
			dispatch_async(dispatch_get_main_queue(), ^{
				
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ON_EXIT_FULLSCREEN object:nil];
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTOP_ANIM object:nil];
				[[MKStoreManager sharedManager] buyFeature:pack4];
			});
		}
		break;
			
		case MK_ACTION_BUY_PACK5:
		{
			dispatch_async(dispatch_get_main_queue(), ^{
				
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ON_EXIT_FULLSCREEN object:nil];
				[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTOP_ANIM object:nil];
				[[MKStoreManager sharedManager] buyFeature:pack5];
			});
		}
		break;*/
	}
	return 1;
}

- (void) dealloc
{
	[super dealloc];
	
	_sharedStoreManager = nil;
	storeObserver = nil;
}

+ (MKStoreManager*) sharedManager
{
	@synchronized(self) {
		
        if (_sharedStoreManager == nil) {
			
            [[self alloc] init]; // assignment not done here
			_sharedStoreManager.purchasableObjects = [[NSMutableArray alloc] init];			
			[_sharedStoreManager requestProductData];
			
			[MKStoreManager loadPurchases];
			_sharedStoreManager.storeObserver = [[MKStoreObserver alloc] init];
			[[SKPaymentQueue defaultQueue] addTransactionObserver:_sharedStoreManager.storeObserver];
        }
    }
    return _sharedStoreManager;
}


#pragma mark Singleton Methods

+ (id)allocWithZone:(NSZone *)zone
{	
    @synchronized(self) {
		
        if (_sharedStoreManager == nil) {
			
            _sharedStoreManager = [super allocWithZone:zone];			
            return _sharedStoreManager;  // assignment and return on first allocation
        }
    }
	
    return nil; //on subsequent allocation attempts return nil	
}


- (id) copyWithZone:(NSZone *)zone
{
    return self;	
}


- (void) requestProductData
{
	SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers: 
								 [NSSet setWithObjects: pack1, pack2, nil]];
	request.delegate = self;
	[request start];
}

- (NSDecimalNumber*) getProductCost_Dollar:(NSString*)productId
{
	for(int i=0;i<[purchasableObjects count];i++)
	{
		SKProduct *product = [purchasableObjects objectAtIndex:i];
		NSString * pId = [product productIdentifier];
		if(pId == nil) {
			continue;
		}
		if([pId isEqualToString:productId])
		{
			return [product price];
		}
	}
	return nil;
}

- (NSString*) getProductCost:(NSString*)productId
{
	for(int i=0;i<[purchasableObjects count];i++)
	{
		SKProduct *product = [purchasableObjects objectAtIndex:i];
		NSString * pId = [product productIdentifier];
		if(pId == nil) {
			continue;
		}
		if([pId isEqualToString:productId])
		{
			NSNumberFormatter * formatter = [[NSNumberFormatter alloc] init];
			[formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
			[formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
			[formatter setLocale:[product priceLocale]];
			NSString * PriceStr = [formatter stringFromNumber:[product price]];
			return PriceStr;
		}
	}
	return nil;
}

- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	[purchasableObjects addObjectsFromArray:response.products];
	for(int i=0 ; i<[purchasableObjects count] ; i++)
	{
		SKProduct * product = [purchasableObjects objectAtIndex:i];
		NSString * pId = [product productIdentifier];
		if(pId == nil) {
			continue;
		}
		NSString * PriceStr = [self getProductCost:pId];
		NSLog(@"Feature: %@, Cost: %@, ID: %@",[product localizedTitle],PriceStr, [product productIdentifier]);
	}
}

- (SKProduct*) findProductById:(NSString*)productId
{
	for(int i=0 ; i<[purchasableObjects count] ; i++)
	{
		SKProduct * product = [purchasableObjects objectAtIndex:i];
		NSString * pId = [product productIdentifier];
		if(pId == nil) {
			continue;
		}
		if([pId isEqualToString:productId] == YES) {
			return product;
		}
	}
	return nil;
}

- (BOOL) restoreCompletedTransactions
{
	if([SKPaymentQueue canMakePayments])
	{
		[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
		return YES;
	}
	NSRunAlertPanel(@"Error", @"You are not authorized to purchase from AppStore", @"OK", nil, nil);
	return NO;
}

- (void) exitInapp
{
//	dispatch_async(dispatch_get_main_queue(), ^{
		[[NSNotificationCenter defaultCenter] postNotificationName:SYS_ONSTART_ANIM object:nil];
		[NSApp activateIgnoringOtherApps:YES];
//	});
}

- (void) buyPack1
{
	[self buyFeature:pack1];
}

- (void) buyPack2
{
	[self buyFeature:pack2];
}
/*
- (void) buyPack3
{
	[self buyFeature:pack3];
}

- (void) buyPack4
{
	[self buyFeature:pack4];
}

- (void) buyPack5
{
	[self buyFeature:pack5];
}
*/
- (void) buyFeature:(NSString*) featureId
{
#if 0
	if([self canCurrentDeviceUseFeature: featureId])
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"MKStoreKit" message:@"You can use this feature for this session."
													   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
		[alert show];
		[alert release];
		
		[self provideContent:featureId /*shouldSerialize:NO*/];
		return;
	}
#endif
	
	if ([SKPaymentQueue canMakePayments])
	{
		SKProduct * product = [self findProductById:featureId];
		if(product)
		{
			SKPayment * payment = [SKPayment paymentWithProduct:product];
			[[SKPaymentQueue defaultQueue] addPayment:payment];
		
			/*
			NSDictionary * Params = [NSDictionary dictionaryWithObjectsAndKeys:
								 @"ProductID", featureId,
								 nil];
			 [[Amplitude instance] logEvent:STAT_INAPP_BUY withEventProperties:Params];
			 */
		}
		else
		{
			NSRunAlertPanel(@"Error", @"Product not found", @"OK", nil, nil);
			[self exitInapp];
		}
	}
	else
	{
		NSLog(@"cant Make Payments");
		NSRunAlertPanel(@"Error", @"You are not authorized to purchase from AppStore", @"OK", nil, nil);
		[self exitInapp];

		/*
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"You are not authorized to purchase from AppStore"
													   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
		[alert show];
		*/
	}
}

- (void) buyFeatureB
{
	//[self buyFeature:featureBId];
}

- (BOOL) getFailedTransaction
{
	if(_lastTransactionFailed == YES) {
		_lastTransactionFailed = NO;
		return YES;
	}
	return NO;
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
	_lastTransactionFailed = YES;
	NSString *messageToBeShown = nil;
	if (transaction.error) {
		NSString * err1 = [transaction.error localizedFailureReason];
		NSString * err2 = [transaction.error localizedRecoverySuggestion];
		if(err1 && err2) {
			messageToBeShown = [NSString stringWithFormat:@"Reason: %@, You can try: %@", err1, err2];
		}
	}
	if(messageToBeShown != nil) {
		NSRunAlertPanel(@"Unable to complete your purchase", messageToBeShown, @"OK", nil, nil);
	}
	
	[self exitInapp];

	/*
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Unable to complete your purchase" message:messageToBeShown
												   delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
	[alert show];*/
}

- (void) clearProvidedContent:(NSString*) productIdentifier
{
	/*
	if([productIdentifier isEqualToString:productIdentifier])
	{
		NSUserDefaults * userDefaults = [NSUserDefaults standardUserDefaults];
		[userDefaults setInteger:0 forKey:productIdentifier];
		[userDefaults synchronize];
	}*/
}

/*
- (int) getProvidedContent:(NSString*) productIdentifier
{
	if([productIdentifier isEqualToString:productIdentifier])
	{
		NSUserDefaults * userDefaults = [NSUserDefaults standardUserDefaults];
		return (int) [userDefaults integerForKey:productIdentifier];
	}
	return 0;
}
*/

- (void) provideContent: (NSString*) productIdentifier
{
	NSLog(@"provideContent");

	if([productIdentifier isEqualToString:pack1]) {
		pack1Purchased = YES;
	}
	if([productIdentifier isEqualToString:pack2]) {
		pack2Purchased = YES;
	}
	/*if([productIdentifier isEqualToString:pack3]) {
		pack3Purchased = YES;
	}
	if([productIdentifier isEqualToString:pack4]) {
		pack4Purchased = YES;
	}
	if([productIdentifier isEqualToString:pack5]) {
		pack5Purchased = YES;
	}*/

	[MKStoreManager updatePurchases];

	[self exitInapp];
}

+ (BOOL) pack1Purchased
{
	return pack1Purchased;
}

+ (BOOL) pack2Purchased
{
	return pack2Purchased;
}
/*
+ (BOOL) pack3Purchased
{
	return pack3Purchased;
}

+ (BOOL) pack4Purchased
{
	return pack4Purchased;
}

+ (BOOL) pack5Purchased
{
	return pack5Purchased;
}
*/

+ (void) loadPurchases 
{
	NSLog(@"loadPurchases");
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];	
	pack1Purchased = [userDefaults boolForKey:pack1];
	pack2Purchased = [userDefaults boolForKey:pack2];
//	pack3Purchased = [userDefaults boolForKey:pack3];
//	pack4Purchased = [userDefaults boolForKey:pack4];
//	pack5Purchased = [userDefaults boolForKey:pack5];
}


+(void) updatePurchases
{
	NSLog(@"updatePurchases");
	NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
	[userDefaults setBool:pack1Purchased forKey:pack1];
	[userDefaults setBool:pack2Purchased forKey:pack2];
//	[userDefaults setBool:pack3Purchased forKey:pack3];
//	[userDefaults setBool:pack4Purchased forKey:pack4];
//	[userDefaults setBool:pack5Purchased forKey:pack5];
}

@end
