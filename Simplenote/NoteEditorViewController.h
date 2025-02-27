//
//  NoteEditorViewController.h
//  Simplenote
//
//  Created by Rainieri Ventura on 2/2/12.
//  Copyright (c) 2012 Simperium. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Note.h"
@import Simperium_OSX;

@class BackgroundView;
@class InterlinkProcessor;
@class NoteEditorViewController;
@class NoteListViewController;
@class MarkdownViewController;
@class Storage;
@class TagsField;
@class ToolbarView;
@class SPTextView;
@class SearchMapView;
@class SearchQuery;
@class NoteEditorMetadataCache;
@protocol EditorControllerDelegate;

NS_ASSUME_NONNULL_BEGIN

#pragma mark - Editor Protocols

@protocol EditorControllerNoteActionsDelegate <NSObject>
- (void)editorController:(NoteEditorViewController *)controller addedNoteWithSimperiumKey:(NSString *)simperiumKey;
- (void)editorController:(NoteEditorViewController *)controller pinnedNoteWithSimperiumKey:(NSString *)simperiumKey;
- (void)editorController:(NoteEditorViewController *)controller restoredNoteWithSimperiumKey:(NSString *)simperiumKey;
- (void)editorController:(NoteEditorViewController *)controller deletedNoteWithSimperiumKey:(NSString *)simperiumKey;
@end

@protocol EditorControllerTagActionsDelegate <NSObject>
- (void)editorController:(NoteEditorViewController *)controller didAddNewTag:(NSString *)tag;
@end



#pragma mark - NoteEditorViewController

@interface NoteEditorViewController : NSViewController

@property (nonatomic, strong) IBOutlet NSMenu                                   *moreActionsMenu;
@property (nonatomic, strong) IBOutlet BackgroundView                           *backgroundView;
@property (nonatomic, strong) IBOutlet NSVisualEffectView                       *headerEffectView;
@property (nonatomic, strong) IBOutlet BackgroundView                           *headerDividerView;
@property (nonatomic, strong) IBOutlet ToolbarView                              *toolbarView;
@property (nonatomic, strong) IBOutlet NSImageView                              *statusImageView;
@property (nonatomic, strong) IBOutlet NSTextField                              *statusTextField;
@property (nonatomic, strong) IBOutlet SPTextView                               *noteEditor;
@property (nonatomic, strong) IBOutlet NSScrollView                             *scrollView;
@property (nonatomic, strong) IBOutlet NSClipView                               *clipView;
@property (nonatomic, strong) IBOutlet NSView                                   *tagsView;
@property (nonatomic, strong) IBOutlet TagsField                                *tagsField;
@property (nonatomic, strong) IBOutlet NSLayoutConstraint                       *editorBottomConstraint;

@property (nonatomic, strong, readonly) MarkdownViewController                  *markdownViewController;
@property (nonatomic, strong, readonly) Storage                                 *storage;
@property (nonatomic, strong, readonly) NSArray<Note *>                         *selectedNotes;
@property (nonatomic, strong, nullable) NSLayoutConstraint                      *sidebarSemaphoreLeadingConstraint;
@property (nonatomic, assign) BOOL                                              viewingTrash;
@property (nonatomic, strong) InterlinkProcessor                                *interlinkProcessor;
@property (nonatomic,   weak) Note                                              *note;
@property (nonatomic,   weak) id<EditorControllerNoteActionsDelegate>           noteActionsDelegate;
@property (nonatomic,   weak) id<EditorControllerTagActionsDelegate>            tagActionsDelegate;
@property (nonatomic,   weak) id<EditorControllerDelegate>                      editorDelegate;
@property (nonatomic, strong, nullable) SearchMapView                           *searchMapView;
@property (nonatomic, strong, nonnull) NoteEditorMetadataCache                  *metadataCache;

// TODO: Switch NSObject >> SearchQuery. ObjC compiler isn't picking up the Swift Package =(
@property (nonatomic, strong, nullable) NSObject                                *searchQuery;

- (IBAction)newNoteWasPressed:(id)sender;
- (IBAction)duplicateNoteWasPressed:(id)sender;
- (void)duplicateCurrentNote;
- (void)save;
- (void)displayNote:(nullable Note *)selectedNote;
- (void)displayNotes:(NSArray<Note *> *)selectedNotes;
- (void)didReceiveNewContent;
- (void)willReceiveNewContent;
- (void)fixChecklistColoring;
- (void)createNoteFromNote:(nullable Note *)oldNote;
- (IBAction)deleteAction:(id)sender;
- (IBAction)printAction:(id)sender;
- (IBAction)adjustFontSizeAction:(id)sender;
- (IBAction)markdownAction:(id)sender;
- (IBAction)toggleMarkdownView:(id)sender;
- (IBAction)insertChecklistAction:(id)sender;

@end

NS_ASSUME_NONNULL_END

