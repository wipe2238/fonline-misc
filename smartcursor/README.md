SmartCursor
===========

SmartCursor is a small class for client-side scripts, allowing to avoid unnecessary clicking when using various items.
Instead of putting "tool" (like hatchet) in active slot, manually changing its mode to "use on" and finally clicking on
resource (like tree), all that is done with single click. Tool required is searched in specified slot, and automagically
used by calling SetChosenActions() function.

Additionally, when hovering usuable item, player is able to see what action will be performed and/or graphic of tool which
is going to be used. Both are prepared to be easily configurable, so if server developer allows that, options could be
changed by players via config file, or using ingame options screen.

By making it as a class, what's major difference between this and original version (see below), feature become a little more
flexible, and - i hope! - easier to use, what was the main goal.

### History ###

Based on FOnline: 2238 script (by JovankaB)

http://fodev.net/forum/index.php/topic,28748.msg248106.html#msg248106
https://github.com/rotators/fo2238/blob/master/Server/scripts/client_smart_cursor.fos

### Setup ###

There are two functions used to configure displaying additional info - DrawInfo(), responsible for settings for drawing
text info, and DrawTool() which takes care of setting for graphic. Please note that calling both functions will overwrite
old setup, if not all arguments are provided. If you want to change just one thing, you need to access specific class member
directly (note that it may be changed in future).

```
void DrawInfo( bool enable, bool toolName = true, int offsetX = 2, int offsetY = -12, uint color = 0, int font = FONT_FALLOUT, int flags = FT_ALIGN | FT_BORDERED );
void DrawTool( bool enable, int offsetX = 3, int offsetY = 5, uint size = 45 );
```

### Collections ###

First step in making cursor smarter is calling Add() function, accepting item PID, PID of tool which can be used on it,
slots to check for, and a string which will be displayed over the item (can be simple text defined in script, or taken from
.msg table). This will create a single 'collection' which will be used later

```
bool Add( uint item, uint tool, array<uint>& slots, string@ text, uint msg, uint idx );
bool Add( uint item, array<uint>& tools, array<uint>& slots, string@ text, uint msg, uint idx );
bool Add( array<uint>& items, uint tool, array<uint>& slots, string@ text, uint msg, uint idx );
bool Add( array<uint>& items, array<uint>& tools, array<uint>& slots, string@ text, uint msg, uint idx );
```

All passed data is checked in exactly same order as in given array, so it's not a problem to use cheaper tool as first
(keeping better one for more harsher times), or searching tools in hands before jumping into inventory. I 

Note about string@ text: as long it's not null, it will be used, and last two arguments (to read info from .msg) ignored.

### Implementing ###

Connecting SmartCursor with reserved client functions is just few lines of code - functions interesting for us are
render_iface() and mouse_down();

There are two Draw() functions prepared, one should be picked depending if script collect chosen during rendering or not.
If not, first variant should be used, otherwise - second. Be warned, that if you enabled drawing tool graphic, __HideCursor
global variable will be modified during rendering, what may bring some problems if it's already changed in other place.

```
void Draw( uint layer );
void Draw( uint layer, CritterCl& chosen );
```

Last function on list is going to be used during mouse click, and will override default action when it's needed.
Returns true if action has been set.

```
bool Click( int click )
```

Complete example:

```
SmartCursor Smarty();

void render_iface( uint layer )
{
    // ... //
    Smarty.Draw( layer );
    // ... //
}

bool mouse_down( int click )
{
    // ... //
    if( Smarty.Click( click ) )
	return( true );
    // ... //
}
```

### Examples ###

Let's chop some wood, shall we?
```
SmartCursor Smarty();

void SmartyInit()
{
    array<uint> slots_hands = { SLOT_HAND1, SLOT_HAND2 };

    array<uint> trees = { PID_TREE1, PID_TREE2, PID_TREE3 };

    Smarty.Add( trees, PID_HATCHET, slots_all, "Chop chop wood", 0, 0 );
}
```

Such piece of code will activate smart cursor on trees with PIDs provided, if character have hatchet in one of hands. I've
took trees here, as they most likely will be first items which have dozens of various pids. If all trees have PID numbers in
some range (for example 100-199), it can be added little easier, keeping same functionality:

```
SmartCursor Smarty();

void SmartyInit()
{
    array<uint> slots_hands = { SLOT_HAND1, SLOT_HAND2 };

    array<uint> trees;
    for( uint t=100; t<200; t++ )
    {
	ProtoItem@ proto = GetProtoItem( t );
	if( valid(proto) )
	    trees.insertLast( t );
    }

    Smarty.Add( trees, PID_HATCHET, slots_all, "Chop chop wood", 0, 0 );
}
```

### Complex example ###

```
SmartCursor Smarty();

void SmartyInit()
{
    // enable text information, disable adding used tool name, change default X offset a bit
    Smarty.DrawInfo( true, false, 5 );

    // disable drawing of tool graphic
    Smarty.DrawTool( false );

    // prepare used arrays
    array<uint> slots_all   = { SLOT_HAND1, SLOT_HAND2, SLOT_INV };
    array<uint> slots_hands = { SLOT_HAND1, SLOT_HAND2 };
    array<uint> slot_active = { SLOT_HAND1 };

    array<uint> trees         = { PID_TREE1, PID_TREE2, PID_TREE3 };
    array<uint> special_trees = { PID_TREE_OF_WISDOM, PID_OLD_TREE };

    array<uint> tools_tree = { PID_PRIMITIVE_TOOL, PID_HATCHET };

    // normal trees will use first primitive tool/hatchet found, no matter what slot it uses
    Smarty.Add( trees, tools_tree, slots_all, "Chop chop wood", 0, 0 );

    // special trees will use hatchet only, must be in hands
    Smarty.Add( special_trees, PID_HATCHET, slots_hands, "Chop chop special wood", 0, 0 );

    // unique tree required to craft Dynamite Launching Crossbow +10 PK -5 Respect,
    // requires hatchet in active slot, displayed text is taken from .msg table
    Smarty.Add( PID_BLOODY_TREE, PID_HATCHET, slot_active, null, TEXTMSG_TEXT, 1207 );
}

void render_iface( uint layer )
{
    // ... //

    Smarty.Draw( layer );

    // ... //
}

bool mouse_down( int click )
{
    // ... //

    if( Smarty.Click( click ))
	return( true );

    // ... //
}
```
