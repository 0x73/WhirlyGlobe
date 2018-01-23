/*
 *  MaplyMapnikVectorTiles.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Jesse Crocker, Trailbehind inc. on 3/31/14.
 *  Copyright 2011-2017 mousebird consulting
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


#import <Foundation/Foundation.h>
#import "MaplyQuadPagingLayer.h"
#import "MaplyTileSource.h"
#import "MaplyCoordinate.h"
#import "MaplyVectorStyle.h"

/** 
    Geometry type for data found within PBF files.
    
    These are the geometry types supported within Mapnik PBF files.
  */
typedef NS_ENUM(NSInteger, MapnikGeometryType) {
  GeomTypeUnknown = 0,
  GeomTypePoint = 1,
  GeomTypeLineString = 2,
  GeomTypePolygon = 3
};

typedef NS_ENUM(NSInteger, MapnikCommandType) {
  SEG_END    = 0,
  SEG_MOVETO = 1,
  SEG_LINETO = 2,
  SEG_CLOSE = (0x40 | 0x0f)
};

@class MaplyVectorTileStyle;
@class MaplyMBTileSource;
@class MaplyRemoteTileInfo;

/** 
    Container for data parsed out of a vector tile.
  */
@interface MaplyVectorTileData : NSObject

/// Component objects already added to the display, but not yet visible.
@property (nonatomic,strong,nullable) NSArray *compObjs;

/// If there were any raster layers, they're here by name
@property (nonatomic,strong,nullable) NSDictionary *rasterLayers;

@end

/** 
    Handles the actual data parsing for an individual vector tile after it comes in.
    
    It you're letting the toolkit do the paging, use a MaplyMapnikVectorTiles which will create one of these.  You only use this directly if you're fetching the data on your own.
  */
@interface MapboxVectorTileParser : NSObject

/// Initialize with the style delegate
- (nonnull instancetype)initWithStyle:(NSObject<MaplyVectorStyleDelegate> *__nonnull)styleDelegate viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC;

/// The styling delegate turns vector data into visible objects in the toolkit
@property (nonatomic, strong, nonnull) NSObject<MaplyVectorStyleDelegate> *styleDelegate;

/// Maply view controller we're adding this data to
@property (nonatomic, weak, nullable) NSObject<MaplyRenderControllerProtocol> * __weak viewC;

/// If set, we'll parse into local coordinates as specified by the bounding box, rather than geo coords
@property (nonatomic, assign) bool localCoords;

@property (nonatomic, assign) BOOL debugLabel;
@property (nonatomic, assign) BOOL debugOutline;

/** 
 Construct the visible objects for the given tile

 @param bbox is in the local coordinate system (likely Spherical Mercator)
 */
- (nullable MaplyVectorTileData *)buildObjects:(NSData *__nonnull)data tile:(MaplyTileID)tileID bounds:(MaplyBoundingBox)bbox geoBounds:(MaplyBoundingBox)geoBbox;

@end

/// The various types of style that will work with Mapnik vector tiles
typedef NS_ENUM(NSInteger, MapnikStyleType) {
	MapnikXMLStyle,
    MapnikSLDStyle
};

/** 
    Provides on demand creation for Mapnik style vector tiles.
    
    Create one of these to read Mapnik PBF style tiles from a remote
    or local source.  This handles the geometry creation, calls a delegate
    for the styling and can read from remote or local data files.
  */
@interface MapboxVectorTiles : NSObject <MaplyPagingDelegate>

/// One or more tile sources to fetch data from per tile
@property (nonatomic, readonly, nullable) NSArray *tileSources;

/// Access token to use with the remote service
@property (nonatomic, strong, nonnull) NSString *accessToken;

/// Handles the actual Mapnik vector tile parsing
@property (nonatomic, strong, nullable) MapboxVectorTileParser *tileParser;

/// Minimum zoom level available
@property (nonatomic, assign) int minZoom;

/// Maximum zoom level available
@property (nonatomic, assign) int maxZoom;

/** 
    A convenience method that fetches all the relevant files and creates a vector tiles object.
    
    This method will fetch all the relevant config files necessary to start a Mapnik vector tile object and the call you back to set up the actual layer.
    
    @param tileSpec Either a local filename or a URL to the remote JSON tile spec.
    
    @param accessToken The access key provided by your service.
    
    @param styleFile Either a local file name or a URL for the Mapnik XML style file.
    
    @param cacheDir Where to cache the vector tiles, or nil for no caching.
    
    @param viewC View controller the data will be associated with.
    
    @param successBlock This block is called with the vector tiles object on success.  You'll need to create the paging layer and attach the vector tiles to it.
    
    @param failureBlock This block is called if any of the loading fails.
  */
+ (void) StartRemoteVectorTilesWithTileSpec:(NSString *__nonnull)tileSpec accessToken:(NSString *__nonnull)accessToken style:(NSString *__nonnull)styleFile styleType:(MapnikStyleType)styleType cacheDir:(NSString *__nonnull)cacheDir viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC success:(void (^__nonnull)(MapboxVectorTiles *__nonnull vecTiles))successBlock failure:(void (^__nonnull)(NSError *__nonnull error))failureBlock;

/** 
    A convenience method that fetches all the relevant files and creates a vector tiles object.
    
    This method will fetch all the relevant config files necessary to start a Mapnik vector tile object and the call you back to set up the actual layer.
    
    @param tileURL The URL to fetch vector tiles from.
    
    @param accessToken The access key provided by your service.
    
    @param ext The tile extension to use.
    
    @param minZoom The minimum zoom level to use.
    
    @param maxZoom The maximum zoom level to use
    
    @param styleFile Either a local file name or a URL for the Mapnik XML style file.
    
    @param cacheDir Where to cache the vector tiles, or nil for no caching.
    
    @param viewC View controller the data will be associated with.
    
    @param successBlock This block is called with the vector tiles object on success.  You'll need to create the paging layer and attach the vector tiles to it.
    
    @param failureBlock This block is called if any of the loading fails.
 */
+ (void) StartRemoteVectorTilesWithURL:(NSString *__nonnull)tileURL ext:(NSString *__nonnull)ext minZoom:(int)minZoom maxZoom:(int)maxZoom accessToken:(NSString *__nonnull)accessToken style:(NSString *__nonnull)styleFile styleType:(MapnikStyleType)styleType cacheDir:(NSString *__nonnull)cacheDir viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC success:(void (^__nonnull)(MapboxVectorTiles *__nonnull vecTiles))successBlock failure:(void (^__nonnull)(NSError *__nonnull error))failureBlock;

/** 
    Init with a single remote tile source.
  */
- (nonnull instancetype)initWithTileSource:(NSObject<MaplyTileSource> *__nonnull)tileSource style:(NSObject<MaplyVectorStyleDelegate> *__nonnull)style viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC;

/** 
    Init with a list of tile sources.
    
    These are MaplyRemoteTileInfo objects and will be combined by the
    MaplyMapnikVectorTiles object for display.
*/
- (nonnull instancetype)initWithTileSources:(NSArray *__nonnull)tileSources style:(NSObject<MaplyVectorStyleDelegate> *__nonnull)style viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC;

/** 
    Init with the filename of an MBTiles archive containing PBF tiles.
    
    This will read individual tiles from an MBTiles archive containging PBF.
    
    The file should be local.
  */
- (nonnull instancetype)initWithMBTiles:(MaplyMBTileSource *__nonnull)tileSource style:(NSObject<MaplyVectorStyleDelegate> *__nonnull)style viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC;

@end

/**
    A tile source for Mapbox vector tiles that renders into images (partially).
 
    This tile source will render some data into images for use by the QuadImages layer and then
  */
@interface MapboxVectorTileImageSource : NSObject<MaplyTileSource>

- (instancetype _Nullable ) initWithTileInfo:(MaplyRemoteTileInfo *_Nonnull)tileInfo style:(NSObject<MaplyVectorStyleDelegate> *__nonnull)style viewC:(NSObject<MaplyRenderControllerProtocol> *__nonnull)viewC;

/// Handles the actual Mapnik vector tile parsing
@property (nonatomic, strong, nullable) MapboxVectorTileParser *tileParser;

@end
