#ifndef KLOHTABP_H
#define KLOGTABP_H

#include "treenew.h"

extern BOOLEAN KLogTreeNewCreated;
extern HWND KLogTreeNewHandle;

#define WEWNTLC_MAXIMUM 4

struct klogstruc
{
    WCHAR timestampstring[20];
    WCHAR PIDstring[8];
    WCHAR ParentPIDstring[8];
    WCHAR ExitCodestring[12];
    LONGLONG timestamp;
    LONGLONG timealive;
    LARGE_INTEGER time;
    DWORD PID;
    DWORD ParentPID;
    NTSTATUS exitcode;
    BYTE startexit;
    PPH_STRING executable;
    PPH_STRING cmdline;
    PPH_STRING TimeAliveText;
};

typedef struct klogstruc *pklog;

typedef struct _WE_KLOG_NODE
{
    PH_TREENEW_NODE Node;

    PH_STRINGREF TextCache[WEWNTLC_MAXIMUM];

    PPH_STRING TimeText;
    struct klogstruc aklog;

} WE_KLOG_NODE, *PWE_KLOG_NODE;

typedef struct _PH_TREENEW_CONTEXT
{
    HWND Handle;
    PVOID InstanceHandle;
    HWND FixedHeaderHandle;
    HWND HeaderHandle;
    HWND VScrollHandle;
    HWND HScrollHandle;
    HWND FillerBoxHandle;
    HWND TooltipsHandle;

    union
    {
        struct
        {
            ULONG FontOwned : 1;
            ULONG Tracking : 1; // tracking for fixed divider
            ULONG VScrollVisible : 1;
            ULONG HScrollVisible : 1;
            ULONG FixedColumnVisible : 1;
            ULONG FixedDividerVisible : 1;
            ULONG AnimateDivider : 1;
            ULONG AnimateDividerFadingIn : 1;
            ULONG AnimateDividerFadingOut : 1;
            ULONG CanAnyExpand : 1;
            ULONG TriState : 1;
            ULONG HasFocus : 1;
            ULONG ThemeInitialized : 1; // delay load theme data
            ULONG ThemeActive : 1;
            ULONG ThemeHasItemBackground : 1;
            ULONG ThemeHasGlyph : 1;
            ULONG ThemeHasHotGlyph : 1;
            ULONG FocusNodeFound : 1; // used to preserve the focused node across restructuring
            ULONG SearchFailed : 1; // used to prevent multiple beeps
            ULONG SearchSingleCharMode : 1; // LV style single-character search
            ULONG TooltipUnfolding : 1; // whether the current tooltip is unfolding
            ULONG DoubleBuffered : 1;
            ULONG SuspendUpdateStructure : 1;
            ULONG SuspendUpdateLayout : 1;
            ULONG SuspendUpdateMoveMouse : 1;
            ULONG DragSelectionActive : 1;
            ULONG SelectionRectangleAlpha : 1; // use alpha blending for the selection rectangle
            ULONG CustomRowHeight : 1;
            ULONG Spare : 4;
        };
        ULONG Flags;
    };
    ULONG Style;
    ULONG ExtendedStyle;
    ULONG ExtendedFlags;

    HFONT Font;
    HCURSOR Cursor;
    HCURSOR DividerCursor;

    RECT ClientRect;
    LONG HeaderHeight;
    LONG RowHeight;
    ULONG VScrollWidth;
    ULONG HScrollHeight;
    LONG VScrollPosition;
    LONG HScrollPosition;
    LONG FixedWidth; // width of the fixed part of the tree list
    LONG FixedWidthMinimum;
    LONG NormalLeft; // FixedWidth + 1 if there is a fixed column, otherwise 0

    PPH_TREENEW_NODE FocusNode;
    ULONG HotNodeIndex;
    ULONG MarkNodeIndex; // selection mark

    ULONG MouseDownLast;
    POINT MouseDownLocation;

    PPH_TREENEW_CALLBACK Callback;
    PVOID CallbackContext;

    PPH_TREENEW_COLUMN *Columns; // columns, indexed by ID
    ULONG NextId;
    ULONG AllocatedColumns;
    ULONG NumberOfColumns; // just a statistic; do not use for actual logic

    PPH_TREENEW_COLUMN *ColumnsByDisplay; // columns, indexed by display order (excluding the fixed column)
    ULONG AllocatedColumnsByDisplay;
    ULONG NumberOfColumnsByDisplay; // the number of visible columns (excluding the fixed column)
    LONG TotalViewX; // total width of normal columns
    PPH_TREENEW_COLUMN FixedColumn;
    PPH_TREENEW_COLUMN FirstColumn; // first column, by display order (including the fixed column)
    PPH_TREENEW_COLUMN LastColumn; // last column, by display order (including the fixed column)

    PPH_TREENEW_COLUMN ResizingColumn;
    LONG OldColumnWidth;
    LONG TrackStartX;
    LONG TrackOldFixedWidth;
    ULONG DividerHot; // 0 for un-hot, 100 for completely hot

    PPH_LIST FlatList;

    ULONG SortColumn; // ID of the column to sort by
    PH_SORT_ORDER SortOrder;

    FLOAT VScrollRemainder;
    FLOAT HScrollRemainder;

    LONG SearchMessageTime;
    PWSTR SearchString;
    ULONG SearchStringCount;
    ULONG AllocatedSearchString;

    ULONG TooltipIndex;
    ULONG TooltipId;
    PPH_STRING TooltipText;
    RECT TooltipRect; // text rectangle of an unfolding tooltip
    HFONT TooltipFont;
    HFONT NewTooltipFont;
    ULONG TooltipColumnId;
    WNDPROC FixedHeaderOldWndProc;
    WNDPROC HeaderOldWndProc;

    TEXTMETRIC TextMetrics;
    HTHEME ThemeData;
    LONG SystemBorderX;
    LONG SystemBorderY;
    LONG SystemEdgeX;
    LONG SystemEdgeY;

    HDC BufferedContext;
    HBITMAP BufferedOldBitmap;
    HBITMAP BufferedBitmap;
    RECT BufferedContextRect;

    LONG SystemDragX;
    LONG SystemDragY;
    RECT DragRect;

    LONG EnableRedraw;
    HRGN SuspendUpdateRegion;

    PH_STRINGREF EmptyText;
} PH_TREENEW_CONTEXT, *PPH_TREENEW_CONTEXT;

void ProcessKLog();
void CreateHwnd();
void UpdateBacklog();

VOID WepAddKLogs(
    _In_ PPH_TREENEW_CONTEXT Context
    );

VOID EtLoadSettingsKLogTreeList(
    VOID
    );

BOOLEAN NTAPI EtpKLogPageCallback(
    _In_ struct _PH_MAIN_TAB_PAGE *Page,
    _In_ PH_MAIN_TAB_PAGE_MESSAGE Message,
    _In_opt_ PVOID Parameter1,
    _In_opt_ PVOID Parameter2
);

VOID NTAPI EtpKlogTabSelectionChangedCallback(
    _In_ PVOID Parameter1,
    _In_ PVOID Parameter2,
    _In_ PVOID Parameter3,
    _In_ PVOID Context
    );

VOID NTAPI EtpKlogTabSaveContentCallback(
    _In_ PVOID Parameter1,
    _In_ PVOID Parameter2,
    _In_ PVOID Parameter3,
    _In_ PVOID Context
    );

VOID NTAPI EtpKlogTabFontChangedCallback(
    _In_ PVOID Parameter1,
    _In_ PVOID Parameter2,
    _In_ PVOID Parameter3,
    _In_ PVOID Context
    );

VOID EtInitializeKLogTreeList(
    _In_ HWND hwnd
    );

BOOLEAN NTAPI EtpKLogTreeNewCallback(
    _In_ HWND hwnd,
    _In_ PH_TREENEW_MESSAGE Message,
    _In_opt_ PVOID Parameter1,
    _In_opt_ PVOID Parameter2,
    _In_opt_ PVOID Context
    );

struct klogstruc *EtGetSelectedKLogItem(
    VOID
    );

VOID EtGetSelectedKLogItems(
    _Out_ pklog **KLogItems,
    _Out_ PULONG NumberOfKLogItems
);

VOID EtDeselectAllKLogNodes(
    VOID
    );

VOID EtSelectAndEnsureVisibleKLogNode(
    _In_ PWE_KLOG_NODE KLogNode
    );

VOID EtCopyKLogList(
    VOID
    );

VOID EtCopyKLogPathList(
    VOID
);

VOID EtWriteKLogList(
    _Inout_ PPH_FILE_STREAM FileStream,
    _In_ ULONG Mode
    );

VOID EtHandleKLogCommand(
    _In_ ULONG Id
    );

VOID EtpInitializeKLogMenu(
    _In_ PPH_EMENU Menu,
    _In_ pklog *KLogItems,
    _In_ ULONG NumberOfKLogItems
);

VOID EtShowKLogContextMenu(
    _In_ POINT Location
    );

VOID NTAPI EtpSearchChangedHandler(
    _In_opt_ PVOID Parameter,
    _In_opt_ PVOID Context
    );

BOOLEAN NTAPI EtpSearchKLogListFilterCallback(
    _In_ PPH_TREENEW_NODE Node,
    _In_opt_ PVOID Context
    );

VOID NTAPI EtpToolStatusActivateContent(
    _In_ BOOLEAN Select
    );

HWND NTAPI EtpToolStatusGetTreeNewHandle(
    VOID
);

INT_PTR CALLBACK EtpKlogTabErrorDialogProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
    );

VOID EtSaveSettingsKLogTreeList(
    VOID
    );

void CleanupDriver();
void StoreExitCode(PPH_PROCESS_ITEM processItem);

#endif
