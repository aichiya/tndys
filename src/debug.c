//CREDITS
//TheXaman:             https://github.com/TheXaman/pokeemerald/tree/tx_debug_system
//CODE USED FROM:
//ketsuban:             https://github.com/pret/pokeemerald/wiki/Add-a-debug-menu
//Pyredrid:             https://github.com/Pyredrid/pokeemerald/tree/debugmenu
//AsparagusEduardo:     https://github.com/AsparagusEduardo/pokeemerald/tree/InfusedEmerald_v2
//Ghoulslash:           https://github.com/ghoulslash/pokeemerald
//Jaizu:                https://jaizu.moe/
#include "global.h"
#include "battle.h"
#include "coins.h"
#include "credits.h"
#include "data.h"
#include "daycare.h"
#include "debug.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "money.h"
#include "naming_screen.h"
#include "new_game.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "region_map.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "pokemon_summary_screen.h"
#include "constants/abilities.h"
#include "constants/battle_frontier.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/map_groups.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/weather.h"

#if DEBUG_OVERWORLD_MENU == TRUE
// *******************************
// Enums
enum { // Main
    DEBUG_MENU_ITEM_UTILITIES,
    DEBUG_MENU_ITEM_SCRIPTS,
    DEBUG_MENU_ITEM_FLAGS,
    DEBUG_MENU_ITEM_VARS,
    DEBUG_MENU_ITEM_GIVE,
    DEBUG_MENU_ITEM_SOUND,
    DEBUG_MENU_ITEM_ACCESS_PC,
    DEBUG_MENU_ITEM_CANCEL
};
enum { // Util
    DEBUG_UTIL_MENU_ITEM_HEAL_PARTY,
    DEBUG_UTIL_MENU_ITEM_FLY,
    DEBUG_UTIL_MENU_ITEM_WARP,
    DEBUG_UTIL_MENU_ITEM_RUNNING_SHOES,
    DEBUG_UTIL_MENU_ITEM_POISON_MONS,
    DEBUG_UTIL_MENU_ITEM_SAVEBLOCK,
    DEBUG_UTIL_MENU_ITEM_WEATHER,
    DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK,
    DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK,
    DEBUG_UTIL_MENU_ITEM_WATCHCREDITS,
    DEBUG_UTIL_MENU_ITEM_TRAINER_NAME,
    DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER,
    DEBUG_UTIL_MENU_ITEM_TRAINER_ID,
};
enum { // Scripts
    DEBUG_UTIL_MENU_ITEM_SCRIPT_1,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_2,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_3,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_4,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_5,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_6,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_7,
    DEBUG_UTIL_MENU_ITEM_SCRIPT_8,
};
enum { // Flags
    DEBUG_FLAG_MENU_ITEM_FLAGS,
    DEBUG_FLAG_MENU_ITEM_POKEDEXFLAGS,
    DEBUG_FLAG_MENU_ITEM_POKEDEXONOFF,
    DEBUG_FLAG_MENU_ITEM_NATDEXONOFF,
    DEBUG_FLAG_MENU_ITEM_POKENAVONOFF,
    DEBUG_FLAG_MENU_ITEM_FLYANYWHERE,
    DEBUG_FLAG_MENU_ITEM_GETALLBADGES,
    DEBUG_FLAG_MENU_ITEM_FRONTIER_PASS,
    DEBUG_FLAG_MENU_ITEM_COLISSION_ONOFF,
    DEBUG_FLAG_MENU_ITEM_ENCOUNTER_ONOFF,
    DEBUG_FLAG_MENU_ITEM_TRAINER_SEE_ONOFF,
    DEBUG_FLAG_MENU_ITEM_BAG_USE_ONOFF,
    DEBUG_FLAG_MENU_ITEM_CATCHING_ONOFF,
};
enum { // Vars
    DEBUG_VARS_MENU_ITEM_VARS,
};
enum { // Give
    DEBUG_GIVE_MENU_ITEM_ITEM_X,
    DEBUG_GIVE_MENU_ITEM_ALLTMS,
    DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE,
    DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX,
    DEBUG_GIVE_MENU_ITEM_MAX_MONEY,
    DEBUG_GIVE_MENU_ITEM_MAX_COINS,
    DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS,
    DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG,
    DEBUG_GIVE_MENU_ITEM_FILL_PC,
    DEBUG_GIVE_MENU_ITEM_CHEAT,
};
enum { //Sound
    DEBUG_SOUND_MENU_ITEM_SE,
    DEBUG_SOUND_MENU_ITEM_MUS,
};

// *******************************
// Constants
#define DEBUG_MAIN_MENU_WIDTH 15
#define DEBUG_MAIN_MENU_HEIGHT 8

#define DEBUG_NUMBER_DISPLAY_WIDTH 10
#define DEBUG_NUMBER_DISPLAY_HEIGHT 4
#define DEBUG_NUMBER_DISPLAY_MEDIUM_WIDTH 15
#define DEBUG_NUMBER_DISPLAY_MEDIUM_HEIGHT 3
#define DEBUG_NUMBER_DISPLAY_SOUND_WIDTH 28
#define DEBUG_NUMBER_DISPLAY_SOUND_HEIGHT 6

#define DEBUG_NUMBER_DIGITS_FLAGS 4
#define DEBUG_NUMBER_DIGITS_VARIABLES 5
#define DEBUG_NUMBER_DIGITS_VARIABLE_VALUE 5
#define DEBUG_NUMBER_DIGITS_ITEMS 4
#define DEBUG_NUMBER_DIGITS_ITEM_QUANTITY 2

#define DEBUG_NUMBER_ICON_X 210
#define DEBUG_NUMBER_ICON_Y 50

// EWRAM
static EWRAM_DATA struct DebugMonData *sDebugMonData = NULL;

// *******************************
struct DebugMonData
{
    u16 mon_speciesId;
    u8  mon_level;
    u8  isShiny;
    u16 mon_natureId;
    u16 mon_abilityNum;
    u8  mon_iv_hp;
    u8  mon_iv_atk;
    u8  mon_iv_def;
    u8  mon_iv_speed;
    u8  mon_iv_satk;
    u8  mon_iv_sdef;
    u16 mon_move_0;
    u16 mon_move_1;
    u16 mon_move_2;
    u16 mon_move_3;
};

// *******************************
// Define functions
static void Debug_ShowMenu(void (*HandleInput)(u8), struct ListMenuTemplate LMtemplate);
void Debug_ShowMainMenu(void);
static void Debug_DestroyMenu(u8 taskId);
static void Debug_DestroyMenu_Full(u8 taskId);
static void DebugAction_Cancel(u8 taskId);
static void DebugAction_DestroyExtraWindow(u8 taskId);

static void DebugAction_Util_Script_1(u8 taskId);
static void DebugAction_Util_Script_2(u8 taskId);
static void DebugAction_Util_Script_3(u8 taskId);
static void DebugAction_Util_Script_4(u8 taskId);
static void DebugAction_Util_Script_5(u8 taskId);
static void DebugAction_Util_Script_6(u8 taskId);
static void DebugAction_Util_Script_7(u8 taskId);
static void DebugAction_Util_Script_8(u8 taskId);

static void DebugAction_OpenUtilitiesMenu(u8 taskId);
static void DebugAction_OpenScriptsMenu(u8 taskId);
static void DebugAction_OpenFlagsMenu(u8 taskId);
static void DebugAction_OpenVariablesMenu(u8 taskId);
static void DebugAction_OpenGiveMenu(u8 taskId);
static void DebugAction_OpenSoundMenu(u8 taskId);
static void DebugTask_HandleMenuInput_Main(u8 taskId);
static void DebugTask_HandleMenuInput_Utilities(u8 taskId);
static void DebugTask_HandleMenuInput_Scripts(u8 taskId);
static void DebugTask_HandleMenuInput_Flags(u8 taskId);
static void DebugTask_HandleMenuInput_Vars(u8 taskId);
static void DebugTask_HandleMenuInput_Give(u8 taskId);
static void DebugTask_HandleMenuInput_Sound(u8 taskId);

static void DebugAction_Util_HealParty(u8 taskId);
static void DebugAction_Util_Fly(u8 taskId);
static void DebugAction_Util_Warp_Warp(u8 taskId);
static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId);
static void DebugAction_Util_Warp_SelectMap(u8 taskId);
static void DebugAction_Util_Warp_SelectWarp(u8 taskId);
static void DebugAction_Util_RunningShoes(u8 taskId);
static void DebugAction_Util_PoisonMons(u8 taskId);
static void DebugAction_Util_CheckSaveBlock(u8 taskId);
static void DebugAction_Util_Weather(u8 taskId);
static void DebugAction_Util_Weather_SelectId(u8 taskId);
static void DebugAction_Util_CheckWallClock(u8 taskId);
static void DebugAction_Util_SetWallClock(u8 taskId);
static void DebugAction_Util_WatchCredits(u8 taskId);
static void DebugAction_Util_Trainer_Name(u8 taskId);
static void DebugAction_Util_Trainer_Gender(u8 taskId);
static void DebugAction_Util_Trainer_Id(u8 taskId);

static void DebugAction_Flags_Flags(u8 taskId);
static void DebugAction_Flags_FlagsSelect(u8 taskId);

static void DebugAction_Flags_SetPokedexFlags(u8 taskId);
static void DebugAction_Flags_SwitchDex(u8 taskId);
static void DebugAction_Flags_SwitchNatDex(u8 taskId);
static void DebugAction_Flags_SwitchPokeNav(u8 taskId);
static void DebugAction_Flags_ToggleFlyFlags(u8 taskId);
static void DebugAction_Flags_ToggleBadgeFlags(u8 taskId);
static void DebugAction_Flags_ToggleFrontierPass(u8 taskId);
static void DebugAction_Flags_CollisionOnOff(u8 taskId);
static void DebugAction_Flags_EncounterOnOff(u8 taskId);
static void DebugAction_Flags_TrainerSeeOnOff(u8 taskId);
static void DebugAction_Flags_BagUseOnOff(u8 taskId);
static void DebugAction_Flags_CatchingOnOff(u8 taskId);

static void DebugAction_Vars_Vars(u8 taskId);
static void DebugAction_Vars_Select(u8 taskId);
static void DebugAction_Vars_SetValue(u8 taskId);

static void DebugAction_Give_Item(u8 taskId);
static void DebugAction_Give_Item_SelectId(u8 taskId);
static void DebugAction_Give_Item_SelectQuantity(u8 taskId);
static void DebugAction_Give_AllTMs(u8 taskId);
static void DebugAction_Give_PokemonSimple(u8 taskId);
static void DebugAction_Give_PokemonComplex(u8 taskId);
static void DebugAction_Give_Pokemon_SelectId(u8 taskId);
static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId);
static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId);
static void DebugAction_Give_Pokemon_SelectNature(u8 taskId);
static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId);
static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId);
static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId);
static void DebugAction_Give_Pokemon_Move(u8 taskId);
static void DebugAction_Give_MaxMoney(u8 taskId);
static void DebugAction_Give_MaxCoins(u8 taskId);
static void DebugAction_Give_MaxBattlePoints(u8 taskId);
static void DebugAction_Give_DayCareEgg(u8 taskId);
static void DebugAction_Give_FillPC(u8 taskId);
static void DebugAction_Give_CHEAT(u8 taskId);
static void DebugAction_AccessPC(u8 taskId);

static void DebugAction_Sound_SE(u8 taskId);
static void DebugAction_Sound_SE_SelectId(u8 taskId);
static void DebugAction_Sound_MUS(u8 taskId);
static void DebugAction_Sound_MUS_SelectId(u8 taskId);

static void DebugTask_HandleMenuInput(u8 taskId, void (*HandleInput)(u8));
static void DebugAction_OpenSubMenu(u8 taskId, struct ListMenuTemplate LMtemplate);

extern u8 Debug_FlagsNotSetMessage[];
extern u8 Debug_Script_1[];
extern u8 Debug_Script_2[];
extern u8 Debug_Script_3[];
extern u8 Debug_Script_4[];
extern u8 Debug_Script_5[];
extern u8 Debug_Script_6[];
extern u8 Debug_Script_7[];
extern u8 Debug_Script_8[];

extern u8 Debug_ShowFieldMessageStringVar4[];
extern u8 Debug_CheatStart[];
extern u8 PlayersHouse_2F_EventScript_SetWallClock[];
extern u8 PlayersHouse_2F_EventScript_CheckWallClock[];

#include "data/map_group_count.h"

// Text
// Main Menu
static const u8 sDebugText_Utilities[] = _("Utilities");
static const u8 sDebugText_Scripts[] =   _("Scripts");
static const u8 sDebugText_Flags[] =     _("Flags");
static const u8 sDebugText_Vars[] =      _("Variables");
static const u8 sDebugText_Give[] =      _("Give X");
static const u8 sDebugText_Sound[] =     _("Sound");
static const u8 sDebugText_Cancel[] =    _("Cancel");
// Script menu
static const u8 sDebugText_Util_Script_1[] = _("Script 1");
static const u8 sDebugText_Util_Script_2[] = _("Script 2");
static const u8 sDebugText_Util_Script_3[] = _("Script 3");
static const u8 sDebugText_Util_Script_4[] = _("Script 4");
static const u8 sDebugText_Util_Script_5[] = _("Script 5");
static const u8 sDebugText_Util_Script_6[] = _("Script 6");
static const u8 sDebugText_Util_Script_7[] = _("Script 7");
static const u8 sDebugText_Util_Script_8[] = _("Script 8");
// Util Menu
static const u8 sDebugText_Util_HealParty[] =                _("Heal Party");
static const u8 sDebugText_Util_Fly[] =                      _("Fly to map");
static const u8 sDebugText_Util_WarpToMap[] =                _("Warp to map warp");
static const u8 sDebugText_Util_WarpToMap_SelectMapGroup[] = _("Group: {STR_VAR_1}          \n                 \n\n{STR_VAR_3}     ");
static const u8 sDebugText_Util_WarpToMap_SelectMap[] =      _("Map: {STR_VAR_1}            \nMapSec:          \n{STR_VAR_2}                       \n{STR_VAR_3}     ");
static const u8 sDebugText_Util_WarpToMap_SelectWarp[] =     _("Warp:             \n{STR_VAR_1}                \n                                  \n{STR_VAR_3}     ");
static const u8 sDebugText_Util_WarpToMap_SelMax[] =         _("{STR_VAR_1} / {STR_VAR_2}");
static const u8 sDebugText_Util_RunningShoes[] =             _("Toggle Running Shoes");
static const u8 sDebugText_Util_PoisonMons[] =               _("Poison all mons");
static const u8 sDebugText_Util_SaveBlockSpace[] =           _("SaveBlock Space");
static const u8 sDebugText_Util_Weather[] =                  _("Set weather");
static const u8 sDebugText_Util_Weather_ID[] =               _("Weather Id: {STR_VAR_3}\n{STR_VAR_1}\n{STR_VAR_2}");
static const u8 sDebugText_Util_CheckWallClock[] =           _("Check Wall Clock");
static const u8 sDebugText_Util_SetWallClock[] =             _("Set Wall Clock");
static const u8 sDebugText_Util_WatchCredits[] =             _("Watch Credits");
static const u8 sDebugText_Util_Trainer_Name[] =             _("Trainer name");
static const u8 sDebugText_Util_Trainer_Gender[] =           _("Toggle T. Gender");
static const u8 sDebugText_Util_Trainer_Id[] =               _("New Trainer Id");
// Flags Menu
static const u8 sDebugText_Flags_Flags[] =              _("Set Flag XXXX");
static const u8 sDebugText_Flags_SetPokedexFlags[] =    _("All Pokédex Flags");
static const u8 sDebugText_Flags_SwitchDex[] =          _("Pokédex ON/OFF");
static const u8 sDebugText_Flags_SwitchNationalDex[] =  _("NatDex ON/OFF");
static const u8 sDebugText_Flags_SwitchPokeNav[] =      _("PokéNav ON/OFF");
static const u8 sDebugText_Flags_ToggleFlyFlags[] =     _("Fly Flags ON/OFF");
static const u8 sDebugText_Flags_ToggleAllBadges[] =    _("All badges ON/OFF");
static const u8 sDebugText_Flags_ToggleFrontierPass[] = _("Frontier Pass ON/OFF");
static const u8 sDebugText_Flags_SwitchCollision[] =    _("Collision ON/OFF");
static const u8 sDebugText_Flags_SwitchEncounter[] =    _("Encounter ON/OFF");
static const u8 sDebugText_Flags_SwitchTrainerSee[] =   _("TrainerSee ON/OFF");
static const u8 sDebugText_Flags_SwitchBagUse[] =       _("BagUse ON/OFF");
static const u8 sDebugText_Flags_SwitchCatching[] =     _("Catching ON/OFF");
static const u8 sDebugText_Flags_Flag[] =               _("Flag: {STR_VAR_1}   \n{STR_VAR_2}                   \n{STR_VAR_3}");
static const u8 sDebugText_Flags_FlagHex[] =            _("{STR_VAR_1}           \n0x{STR_VAR_2}             ");
static const u8 sDebugText_Flags_FlagSet[] =            _("TRUE");
static const u8 sDebugText_Flags_FlagUnset[] =          _("FALSE");
// Variables Menu
static const u8 sDebugText_Vars_Vars[] =             _("Set Vars XXXX");
static const u8 sDebugText_Vars_VariableHex[] =      _("{STR_VAR_1}           \n0x{STR_VAR_2}             ");
static const u8 sDebugText_Vars_Variable[] =         _("Var: {STR_VAR_1}             \nVal: {STR_VAR_3}             \n{STR_VAR_2}");
static const u8 sDebugText_Vars_VariableValueSet[] = _("Var: {STR_VAR_1}             \nVal: {STR_VAR_3}             \n{STR_VAR_2}");
// Give Menu
static const u8 sDebugText_Give_GiveItem[] =           _("Give item XXXX");
static const u8 sDebugText_ItemQuantity[] =            _("Quantity:       \n{STR_VAR_1}    \n\n{STR_VAR_2}");
static const u8 sDebugText_ItemID[] =                  _("Item Id: {STR_VAR_3}\n{STR_VAR_1}    \n\n{STR_VAR_2}");
static const u8 sDebugText_Give_AllTMs[] =             _("Give all TMs");
static const u8 sDebugText_Give_GivePokemonSimple[] =  _("Pkm(lvl)");
static const u8 sDebugText_Give_GivePokemonComplex[] = _("Pkm(l,s,n,a,IV,mov)");
static const u8 sDebugText_PokemonID[] =               _("Species: {STR_VAR_3}\n{STR_VAR_1}    \n\n{STR_VAR_2}");
static const u8 sDebugText_PokemonLevel[] =            _("Level:                   \n{STR_VAR_1}           \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonShiny[] =            _("Shiny:                   \n   {STR_VAR_2}             \n              \n                ");
static const u8 sDebugText_PokemonNature[] =           _("NatureId: {STR_VAR_3}          \n{STR_VAR_1}          \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonAbility[] =          _("AbilityNum: {STR_VAR_3}          \n{STR_VAR_1}          \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonIVs[] =              _("All IVs:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_0[] =             _("IV HP:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_1[] =             _("IV Attack:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_2[] =             _("IV Defense:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_3[] =             _("IV Speed:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_4[] =             _("IV Sp. Attack:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonIV_5[] =             _("IV Sp. Defense:               \n    {STR_VAR_3}            \n             \n{STR_VAR_2}          ");
static const u8 sDebugText_PokemonMove_0[] =           _("Move 0: {STR_VAR_3}                   \n{STR_VAR_1}           \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonMove_1[] =           _("Move 1: {STR_VAR_3}                   \n{STR_VAR_1}           \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonMove_2[] =           _("Move 2: {STR_VAR_3}                   \n{STR_VAR_1}           \n          \n{STR_VAR_2}");
static const u8 sDebugText_PokemonMove_3[] =           _("Move 3: {STR_VAR_3}                   \n{STR_VAR_1}           \n          \n{STR_VAR_2}");
static const u8 sDebugText_Give_MaxMoney[] =           _("Max Money");
static const u8 sDebugText_Give_MaxCoins[] =           _("Max Coins");
static const u8 sDebugText_Give_BattlePoints[] =       _("Max Battle Points");
static const u8 sDebugText_Give_DaycareEgg[] =         _("Daycare Egg");
static const u8 sDebugText_Give_FillPc[] =             _("Fill Pc");
static const u8 sDebugText_Give_GiveCHEAT[] =          _("CHEAT Start");
static const u8 sDebugText_AccessPC[] =                _("Access PC");
// Sound Mneu
static const u8 sDebugText_Sound_SE[] =     _("Effects");
static const u8 sDebugText_Sound_SE_ID[] =  _("Sound Id: {STR_VAR_3}\n{STR_VAR_1}    \n{STR_VAR_2}");
static const u8 sDebugText_Sound_MUS[] =    _("Music");
static const u8 sDebugText_Sound_MUS_ID[] = _("Music Id: {STR_VAR_3}\n{STR_VAR_1}    \n{STR_VAR_2}");
static const u8 sDebugText_Sound_Empty[] =  _("");

static const u8 digitInidicator_1[] =        _("{LEFT_ARROW}+1{RIGHT_ARROW}        ");
static const u8 digitInidicator_10[] =       _("{LEFT_ARROW}+10{RIGHT_ARROW}       ");
static const u8 digitInidicator_100[] =      _("{LEFT_ARROW}+100{RIGHT_ARROW}      ");
static const u8 digitInidicator_1000[] =     _("{LEFT_ARROW}+1000{RIGHT_ARROW}     ");
static const u8 digitInidicator_10000[] =    _("{LEFT_ARROW}+10000{RIGHT_ARROW}    ");
static const u8 digitInidicator_100000[] =   _("{LEFT_ARROW}+100000{RIGHT_ARROW}   ");
static const u8 digitInidicator_1000000[] =  _("{LEFT_ARROW}+1000000{RIGHT_ARROW}  ");
static const u8 digitInidicator_10000000[] = _("{LEFT_ARROW}+10000000{RIGHT_ARROW} ");
const u8 * const gText_DigitIndicator[] =
{
    digitInidicator_1,
    digitInidicator_10,
    digitInidicator_100,
    digitInidicator_1000,
    digitInidicator_10000,
    digitInidicator_100000,
    digitInidicator_1000000,
    digitInidicator_10000000
};
static const s32 sPowersOfTen[] =
{
             1,
            10,
           100,
          1000,
         10000,
        100000,
       1000000,
      10000000,
     100000000,
    1000000000,
};

// *******************************
// List Menu Items
static const struct ListMenuItem sDebugMenu_Items_Main[] =
{
    [DEBUG_MENU_ITEM_UTILITIES] = {sDebugText_Utilities, DEBUG_MENU_ITEM_UTILITIES},
    [DEBUG_MENU_ITEM_SCRIPTS]   = {sDebugText_Scripts,   DEBUG_MENU_ITEM_SCRIPTS},
    [DEBUG_MENU_ITEM_FLAGS]     = {sDebugText_Flags,     DEBUG_MENU_ITEM_FLAGS},
    [DEBUG_MENU_ITEM_VARS]      = {sDebugText_Vars,      DEBUG_MENU_ITEM_VARS},
    [DEBUG_MENU_ITEM_GIVE]      = {sDebugText_Give,      DEBUG_MENU_ITEM_GIVE},
    [DEBUG_MENU_ITEM_SOUND]     = {sDebugText_Sound,     DEBUG_MENU_ITEM_SOUND},
    [DEBUG_MENU_ITEM_ACCESS_PC] = {sDebugText_AccessPC,  DEBUG_MENU_ITEM_ACCESS_PC},
    [DEBUG_MENU_ITEM_CANCEL]    = {sDebugText_Cancel,    DEBUG_MENU_ITEM_CANCEL}
};
static const struct ListMenuItem sDebugMenu_Items_Utilities[] =
{
    [DEBUG_UTIL_MENU_ITEM_HEAL_PARTY]     = {sDebugText_Util_HealParty,      DEBUG_UTIL_MENU_ITEM_HEAL_PARTY},
    [DEBUG_UTIL_MENU_ITEM_FLY]            = {sDebugText_Util_Fly,            DEBUG_UTIL_MENU_ITEM_FLY},
    [DEBUG_UTIL_MENU_ITEM_WARP]           = {sDebugText_Util_WarpToMap,      DEBUG_UTIL_MENU_ITEM_WARP},
    [DEBUG_UTIL_MENU_ITEM_RUNNING_SHOES]  = {sDebugText_Util_RunningShoes,   DEBUG_UTIL_MENU_ITEM_RUNNING_SHOES},
    [DEBUG_UTIL_MENU_ITEM_POISON_MONS]    = {sDebugText_Util_PoisonMons,     DEBUG_UTIL_MENU_ITEM_POISON_MONS},
    [DEBUG_UTIL_MENU_ITEM_SAVEBLOCK]      = {sDebugText_Util_SaveBlockSpace, DEBUG_UTIL_MENU_ITEM_SAVEBLOCK},
    [DEBUG_UTIL_MENU_ITEM_WEATHER]        = {sDebugText_Util_Weather,        DEBUG_UTIL_MENU_ITEM_WEATHER},
    [DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK] = {sDebugText_Util_CheckWallClock, DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK},
    [DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK]   = {sDebugText_Util_SetWallClock,   DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK},
    [DEBUG_UTIL_MENU_ITEM_WATCHCREDITS]   = {sDebugText_Util_WatchCredits,   DEBUG_UTIL_MENU_ITEM_WATCHCREDITS},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_NAME]   = {sDebugText_Util_Trainer_Name,   DEBUG_UTIL_MENU_ITEM_TRAINER_NAME},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER] = {sDebugText_Util_Trainer_Gender, DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER},
    [DEBUG_UTIL_MENU_ITEM_TRAINER_ID]     = {sDebugText_Util_Trainer_Id,     DEBUG_UTIL_MENU_ITEM_TRAINER_ID},
};
static const struct ListMenuItem sDebugMenu_Items_Scripts[] =
{
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_1] = {sDebugText_Util_Script_1, DEBUG_UTIL_MENU_ITEM_SCRIPT_1},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_2] = {sDebugText_Util_Script_2, DEBUG_UTIL_MENU_ITEM_SCRIPT_2},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_3] = {sDebugText_Util_Script_3, DEBUG_UTIL_MENU_ITEM_SCRIPT_3},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_4] = {sDebugText_Util_Script_4, DEBUG_UTIL_MENU_ITEM_SCRIPT_4},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_5] = {sDebugText_Util_Script_5, DEBUG_UTIL_MENU_ITEM_SCRIPT_5},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_6] = {sDebugText_Util_Script_6, DEBUG_UTIL_MENU_ITEM_SCRIPT_6},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_7] = {sDebugText_Util_Script_7, DEBUG_UTIL_MENU_ITEM_SCRIPT_7},
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_8] = {sDebugText_Util_Script_8, DEBUG_UTIL_MENU_ITEM_SCRIPT_8},
};
static const struct ListMenuItem sDebugMenu_Items_Flags[] =
{
    [DEBUG_FLAG_MENU_ITEM_FLAGS]             = {sDebugText_Flags_Flags,              DEBUG_FLAG_MENU_ITEM_FLAGS},
    [DEBUG_FLAG_MENU_ITEM_POKEDEXFLAGS]      = {sDebugText_Flags_SetPokedexFlags,    DEBUG_FLAG_MENU_ITEM_POKEDEXFLAGS},
    [DEBUG_FLAG_MENU_ITEM_POKEDEXONOFF]      = {sDebugText_Flags_SwitchDex,          DEBUG_FLAG_MENU_ITEM_POKEDEXONOFF},
    [DEBUG_FLAG_MENU_ITEM_NATDEXONOFF]       = {sDebugText_Flags_SwitchNationalDex,  DEBUG_FLAG_MENU_ITEM_NATDEXONOFF},
    [DEBUG_FLAG_MENU_ITEM_POKENAVONOFF]      = {sDebugText_Flags_SwitchPokeNav,      DEBUG_FLAG_MENU_ITEM_POKENAVONOFF},
    [DEBUG_FLAG_MENU_ITEM_FLYANYWHERE]       = {sDebugText_Flags_ToggleFlyFlags,     DEBUG_FLAG_MENU_ITEM_FLYANYWHERE},
    [DEBUG_FLAG_MENU_ITEM_GETALLBADGES]      = {sDebugText_Flags_ToggleAllBadges,    DEBUG_FLAG_MENU_ITEM_GETALLBADGES},
    [DEBUG_FLAG_MENU_ITEM_FRONTIER_PASS]     = {sDebugText_Flags_ToggleFrontierPass, DEBUG_FLAG_MENU_ITEM_FRONTIER_PASS},
    [DEBUG_FLAG_MENU_ITEM_COLISSION_ONOFF]   = {sDebugText_Flags_SwitchCollision,    DEBUG_FLAG_MENU_ITEM_COLISSION_ONOFF},
    [DEBUG_FLAG_MENU_ITEM_ENCOUNTER_ONOFF]   = {sDebugText_Flags_SwitchEncounter,    DEBUG_FLAG_MENU_ITEM_ENCOUNTER_ONOFF},
    [DEBUG_FLAG_MENU_ITEM_TRAINER_SEE_ONOFF] = {sDebugText_Flags_SwitchTrainerSee,   DEBUG_FLAG_MENU_ITEM_TRAINER_SEE_ONOFF},
    [DEBUG_FLAG_MENU_ITEM_BAG_USE_ONOFF]     = {sDebugText_Flags_SwitchBagUse,       DEBUG_FLAG_MENU_ITEM_BAG_USE_ONOFF},
    [DEBUG_FLAG_MENU_ITEM_CATCHING_ONOFF]    = {sDebugText_Flags_SwitchCatching,     DEBUG_FLAG_MENU_ITEM_CATCHING_ONOFF},
};
static const struct ListMenuItem sDebugMenu_Items_Vars[] =
{
    [DEBUG_VARS_MENU_ITEM_VARS] = {sDebugText_Vars_Vars, DEBUG_FLAG_MENU_ITEM_FLAGS},
};
static const struct ListMenuItem sDebugMenu_Items_Give[] =
{
    [DEBUG_GIVE_MENU_ITEM_ITEM_X]            = {sDebugText_Give_GiveItem,           DEBUG_GIVE_MENU_ITEM_ITEM_X},
    [DEBUG_GIVE_MENU_ITEM_ALLTMS]            = {sDebugText_Give_AllTMs,             DEBUG_GIVE_MENU_ITEM_ALLTMS},
    [DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE]    = {sDebugText_Give_GivePokemonSimple,  DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE},
    [DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX]   = {sDebugText_Give_GivePokemonComplex, DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX},
    [DEBUG_GIVE_MENU_ITEM_MAX_MONEY]         = {sDebugText_Give_MaxMoney,           DEBUG_GIVE_MENU_ITEM_MAX_MONEY},
    [DEBUG_GIVE_MENU_ITEM_MAX_COINS]         = {sDebugText_Give_MaxCoins,           DEBUG_GIVE_MENU_ITEM_MAX_COINS},
    [DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS] = {sDebugText_Give_BattlePoints,       DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS},
    [DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG]       = {sDebugText_Give_DaycareEgg,         DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG},
    [DEBUG_GIVE_MENU_ITEM_FILL_PC]           = {sDebugText_Give_FillPc,             DEBUG_GIVE_MENU_ITEM_FILL_PC},
    [DEBUG_GIVE_MENU_ITEM_CHEAT]             = {sDebugText_Give_GiveCHEAT,          DEBUG_GIVE_MENU_ITEM_CHEAT},
};
static const struct ListMenuItem sDebugMenu_Items_Sound[] =
{
    [DEBUG_SOUND_MENU_ITEM_SE]  = {sDebugText_Sound_SE,  DEBUG_SOUND_MENU_ITEM_SE},
    [DEBUG_SOUND_MENU_ITEM_MUS] = {sDebugText_Sound_MUS, DEBUG_SOUND_MENU_ITEM_MUS},
};

// *******************************
// Menu Actions
static void (*const sDebugMenu_Actions_Main[])(u8) =
{
    [DEBUG_MENU_ITEM_UTILITIES] = DebugAction_OpenUtilitiesMenu,
    [DEBUG_MENU_ITEM_SCRIPTS]   = DebugAction_OpenScriptsMenu,
    [DEBUG_MENU_ITEM_FLAGS]     = DebugAction_OpenFlagsMenu,
    [DEBUG_MENU_ITEM_VARS]      = DebugAction_OpenVariablesMenu,
    [DEBUG_MENU_ITEM_GIVE]      = DebugAction_OpenGiveMenu,
    [DEBUG_MENU_ITEM_SOUND]     = DebugAction_OpenSoundMenu,
    [DEBUG_MENU_ITEM_ACCESS_PC] = DebugAction_AccessPC,
    [DEBUG_MENU_ITEM_CANCEL]    = DebugAction_Cancel
};
static void (*const sDebugMenu_Actions_Utilities[])(u8) =
{
    [DEBUG_UTIL_MENU_ITEM_HEAL_PARTY]     = DebugAction_Util_HealParty,
    [DEBUG_UTIL_MENU_ITEM_FLY]            = DebugAction_Util_Fly,
    [DEBUG_UTIL_MENU_ITEM_WARP]           = DebugAction_Util_Warp_Warp,
    [DEBUG_UTIL_MENU_ITEM_RUNNING_SHOES]  = DebugAction_Util_RunningShoes,
    [DEBUG_UTIL_MENU_ITEM_POISON_MONS]    = DebugAction_Util_PoisonMons,
    [DEBUG_UTIL_MENU_ITEM_SAVEBLOCK]      = DebugAction_Util_CheckSaveBlock,
    [DEBUG_UTIL_MENU_ITEM_WEATHER]        = DebugAction_Util_Weather,
    [DEBUG_UTIL_MENU_ITEM_CHECKWALLCLOCK] = DebugAction_Util_CheckWallClock,
    [DEBUG_UTIL_MENU_ITEM_SETWALLCLOCK]   = DebugAction_Util_SetWallClock,
    [DEBUG_UTIL_MENU_ITEM_WATCHCREDITS]   = DebugAction_Util_WatchCredits,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_NAME]   = DebugAction_Util_Trainer_Name,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_GENDER] = DebugAction_Util_Trainer_Gender,
    [DEBUG_UTIL_MENU_ITEM_TRAINER_ID]     = DebugAction_Util_Trainer_Id,
};
static void (*const sDebugMenu_Actions_Scripts[])(u8) =
{
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_1] = DebugAction_Util_Script_1,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_2] = DebugAction_Util_Script_2,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_3] = DebugAction_Util_Script_3,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_4] = DebugAction_Util_Script_4,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_5] = DebugAction_Util_Script_5,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_6] = DebugAction_Util_Script_6,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_7] = DebugAction_Util_Script_7,
    [DEBUG_UTIL_MENU_ITEM_SCRIPT_8] = DebugAction_Util_Script_8,
};
static void (*const sDebugMenu_Actions_Flags[])(u8) =
{
    [DEBUG_FLAG_MENU_ITEM_FLAGS]             = DebugAction_Flags_Flags,
    [DEBUG_FLAG_MENU_ITEM_POKEDEXFLAGS]      = DebugAction_Flags_SetPokedexFlags,
    [DEBUG_FLAG_MENU_ITEM_POKEDEXONOFF]      = DebugAction_Flags_SwitchDex,
    [DEBUG_FLAG_MENU_ITEM_NATDEXONOFF]       = DebugAction_Flags_SwitchNatDex,
    [DEBUG_FLAG_MENU_ITEM_POKENAVONOFF]      = DebugAction_Flags_SwitchPokeNav,
    [DEBUG_FLAG_MENU_ITEM_FLYANYWHERE]       = DebugAction_Flags_ToggleFlyFlags,
    [DEBUG_FLAG_MENU_ITEM_GETALLBADGES]      = DebugAction_Flags_ToggleBadgeFlags,
    [DEBUG_FLAG_MENU_ITEM_FRONTIER_PASS]     = DebugAction_Flags_ToggleFrontierPass,
    [DEBUG_FLAG_MENU_ITEM_COLISSION_ONOFF]   = DebugAction_Flags_CollisionOnOff,
    [DEBUG_FLAG_MENU_ITEM_ENCOUNTER_ONOFF]   = DebugAction_Flags_EncounterOnOff,
    [DEBUG_FLAG_MENU_ITEM_TRAINER_SEE_ONOFF] = DebugAction_Flags_TrainerSeeOnOff,
    [DEBUG_FLAG_MENU_ITEM_BAG_USE_ONOFF]     = DebugAction_Flags_BagUseOnOff,
    [DEBUG_FLAG_MENU_ITEM_CATCHING_ONOFF]    = DebugAction_Flags_CatchingOnOff,
};
static void (*const sDebugMenu_Actions_Vars[])(u8) =
{
    [DEBUG_VARS_MENU_ITEM_VARS] = DebugAction_Vars_Vars,
};
static void (*const sDebugMenu_Actions_Give[])(u8) =
{
    [DEBUG_GIVE_MENU_ITEM_ITEM_X]            = DebugAction_Give_Item,
    [DEBUG_GIVE_MENU_ITEM_ALLTMS]            = DebugAction_Give_AllTMs,
    [DEBUG_GIVE_MENU_ITEM_POKEMON_SIMPLE]    = DebugAction_Give_PokemonSimple,
    [DEBUG_GIVE_MENU_ITEM_POKEMON_COMPLEX]   = DebugAction_Give_PokemonComplex,
    [DEBUG_GIVE_MENU_ITEM_MAX_MONEY]         = DebugAction_Give_MaxMoney,
    [DEBUG_GIVE_MENU_ITEM_MAX_COINS]         = DebugAction_Give_MaxCoins,
    [DEBUG_GIVE_MENU_ITEM_MAX_BATTLE_POINTS] = DebugAction_Give_MaxBattlePoints,
    [DEBUG_GIVE_MENU_ITEM_DAYCARE_EGG]       = DebugAction_Give_DayCareEgg,
    [DEBUG_GIVE_MENU_ITEM_FILL_PC]           = DebugAction_Give_FillPC,
    [DEBUG_GIVE_MENU_ITEM_CHEAT]             = DebugAction_Give_CHEAT,
};
static void (*const sDebugMenu_Actions_Sound[])(u8) =
{
    [DEBUG_SOUND_MENU_ITEM_SE]  = DebugAction_Sound_SE,
    [DEBUG_SOUND_MENU_ITEM_MUS] = DebugAction_Sound_MUS,
};


// *******************************
// Windows
static const struct WindowTemplate sDebugMenuWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = DEBUG_MAIN_MENU_WIDTH,
    .height = 2 * DEBUG_MAIN_MENU_HEIGHT,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugNumberDisplayWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 4 + DEBUG_MAIN_MENU_WIDTH,
    .tilemapTop = 1,
    .width = DEBUG_NUMBER_DISPLAY_WIDTH,
    .height = 2 * DEBUG_NUMBER_DISPLAY_HEIGHT,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugNumberDisplayMediumWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_NUMBER_DISPLAY_MEDIUM_WIDTH - 1,
    .tilemapTop = 1,
    .width = DEBUG_NUMBER_DISPLAY_MEDIUM_WIDTH,
    .height = 2 * DEBUG_NUMBER_DISPLAY_MEDIUM_HEIGHT,
    .paletteNum = 15,
    .baseBlock = 1,
};
static const struct WindowTemplate sDebugNumberDisplayLargeWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_NUMBER_DISPLAY_SOUND_WIDTH -1,
    .tilemapTop = 1,
    .width = DEBUG_NUMBER_DISPLAY_SOUND_WIDTH,
    .height = DEBUG_NUMBER_DISPLAY_SOUND_HEIGHT,
    .paletteNum = 15,
    .baseBlock = 1,
};

// *******************************
// List Menu Templates
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Main =
{
    .items = sDebugMenu_Items_Main,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Main),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Utilities =
{
    .items = sDebugMenu_Items_Utilities,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Utilities),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Scripts =
{
    .items = sDebugMenu_Items_Scripts,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Scripts),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Flags =
{
    .items = sDebugMenu_Items_Flags,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Flags),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Vars =
{
    .items = sDebugMenu_Items_Vars,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Vars),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Give =
{
    .items = sDebugMenu_Items_Give,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Give),
};
static const struct ListMenuTemplate sDebugMenu_ListTemplate_Sound =
{
    .items = sDebugMenu_Items_Sound,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .totalItems = ARRAY_COUNT(sDebugMenu_Items_Sound),
};


// *******************************
// Functions universal
void Debug_ShowMainMenu(void)
{
    Debug_ShowMenu(DebugTask_HandleMenuInput_Main, sDebugMenu_ListTemplate_Main);
}
static void Debug_ShowMenu(void (*HandleInput)(u8), struct ListMenuTemplate LMtemplate)
{
    struct ListMenuTemplate menuTemplate;
    u8 windowId;
    u8 menuTaskId;
    u8 inputTaskId;

    // create window
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    // create list menu
    menuTemplate = LMtemplate;
    menuTemplate.maxShowed = DEBUG_MAIN_MENU_HEIGHT;
    menuTemplate.windowId = windowId;
    menuTemplate.header_X = 0;
    menuTemplate.item_X = 8;
    menuTemplate.cursor_X = 0;
    menuTemplate.upText_Y = 1;
    menuTemplate.cursorPal = 2;
    menuTemplate.fillValue = 1;
    menuTemplate.cursorShadowPal = 3;
    menuTemplate.lettersSpacing = 1;
    menuTemplate.itemVerticalPadding = 0;
    menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    menuTemplate.fontId = 1;
    menuTemplate.cursorKind = 0;
    menuTaskId = ListMenuInit(&menuTemplate, 0, 0);

    // draw everything
    CopyWindowToVram(windowId, 3);

    // create input handler task
    inputTaskId = CreateTask(HandleInput, 3);
    gTasks[inputTaskId].data[0] = menuTaskId;
    gTasks[inputTaskId].data[1] = windowId;
}
static void Debug_DestroyMenu(u8 taskId)
{
    DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
    RemoveWindow(gTasks[taskId].data[1]);
    DestroyTask(taskId);
}
static void Debug_DestroyMenu_Full(u8 taskId)
{
    DestroyListMenuTask(gTasks[taskId].data[0], NULL, NULL);
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);
    DestroyTask(taskId);
    UnfreezeObjectEvents();
}
static void DebugAction_Cancel(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_DestroyExtraWindow(u8 taskId)
{
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    ClearStdWindowAndFrame(gTasks[taskId].data[2], TRUE);
    RemoveWindow(gTasks[taskId].data[2]);

    DestroyTask(taskId);
    ScriptContext_Enable();
    UnfreezeObjectEvents();
}


// *******************************
// Handle Inputs
static void DebugTask_HandleMenuInput_Main(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Main[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu_Full(taskId);
        ScriptContext_Enable();
    }
}
static void DebugTask_HandleMenuInput_Utilities(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Utilities[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Scripts(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Scripts[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Flags(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Flags[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Vars(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Vars[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Give(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Give[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}
static void DebugTask_HandleMenuInput_Sound(u8 taskId)
{
    void (*func)(u8);
    u32 input = ListMenu_ProcessInput(gTasks[taskId].data[0]);

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if ((func = sDebugMenu_Actions_Sound[input]) != NULL)
            func(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Debug_DestroyMenu(taskId);
        Debug_ShowMainMenu();
    }
}

// *******************************
// Open sub-menus
static void DebugAction_OpenUtilitiesMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Utilities, sDebugMenu_ListTemplate_Utilities);
}
static void DebugAction_OpenScriptsMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Scripts, sDebugMenu_ListTemplate_Scripts);
}
static void DebugAction_OpenFlagsMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Flags, sDebugMenu_ListTemplate_Flags);
}
static void DebugAction_OpenVariablesMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Vars, sDebugMenu_ListTemplate_Vars);
}
static void DebugAction_OpenGiveMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Give, sDebugMenu_ListTemplate_Give);
}
static void DebugAction_OpenSoundMenu(u8 taskId)
{
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_Sound, sDebugMenu_ListTemplate_Sound);
}


// *******************************
// Actions Utilities
static void DebugAction_Util_HealParty(u8 taskId)
{
    PlaySE(SE_USE_ITEM);
    HealPlayerParty();
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}
static void DebugAction_Util_Fly(u8 taskId)
{
    FlagSet(FLAG_VISITED_LITTLEROOT_TOWN);
    FlagSet(FLAG_VISITED_OLDALE_TOWN);
    FlagSet(FLAG_VISITED_DEWFORD_TOWN);
    FlagSet(FLAG_VISITED_LAVARIDGE_TOWN);
    FlagSet(FLAG_VISITED_FALLARBOR_TOWN);
    FlagSet(FLAG_VISITED_VERDANTURF_TOWN);
    FlagSet(FLAG_VISITED_PACIFIDLOG_TOWN);
    FlagSet(FLAG_VISITED_PETALBURG_CITY);
    FlagSet(FLAG_VISITED_SLATEPORT_CITY);
    FlagSet(FLAG_VISITED_MAUVILLE_CITY);
    FlagSet(FLAG_VISITED_RUSTBORO_CITY);
    FlagSet(FLAG_VISITED_FORTREE_CITY);
    FlagSet(FLAG_VISITED_LILYCOVE_CITY);
    FlagSet(FLAG_VISITED_MOSSDEEP_CITY);
    FlagSet(FLAG_VISITED_SOOTOPOLIS_CITY);
    FlagSet(FLAG_VISITED_EVER_GRANDE_CITY);
    FlagSet(FLAG_LANDMARK_POKEMON_LEAGUE);
    FlagSet(FLAG_LANDMARK_BATTLE_FRONTIER);
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_OpenFlyMap);
}

static void DebugAction_Util_Warp_Warp(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
    ConvertIntToDecimalStringN(gStringVar2, MAP_GROUPS_COUNT-1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
    StringCopy(gStringVar3, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Warp_SelectMapGroup;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 0;            //Current Flag
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 0; //Map Group
    gTasks[taskId].data[6] = 0; //Map
    gTasks[taskId].data[7] = 0; //warp
}
static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > MAP_GROUPS_COUNT - 1)
                gTasks[taskId].data[3] = MAP_GROUPS_COUNT - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUPS_COUNT - 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].data[5]] - 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].data[5], gTasks[taskId].data[3])->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Util_Warp_SelectMap;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Util_Warp_SelectMap(u8 taskId)
{
    u8 max_value = MAP_GROUP_COUNT[gTasks[taskId].data[5]]; //maps in the selected map group

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > max_value - 1)
                gTasks[taskId].data[3] = max_value - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].data[5]] - 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].data[5], gTasks[taskId].data[3])->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].data[6] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
        gTasks[taskId].func = DebugAction_Util_Warp_SelectWarp;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Util_Warp_SelectWarp(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 10)
                gTasks[taskId].data[3] = 10;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].data[7] = gTasks[taskId].data[3];
        //WARP
        //If there's no warp with the number available, warp to the center of the map.
        SetWarpDestinationToMapWarp(gTasks[taskId].data[5], gTasks[taskId].data[6], gTasks[taskId].data[7]);
        DoWarp();
        ResetInitialPlayerAvatarState();
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_RunningShoes(u8 taskId)
{
    if (FlagGet(FLAG_SYS_B_DASH))
    {
        FlagClear(FLAG_SYS_B_DASH);
        PlaySE(SE_PC_OFF);
    }
    else
    {
        FlagSet(FLAG_SYS_B_DASH);
        PlaySE(SE_PC_LOGIN);
    }
}

static void DebugAction_Util_PoisonMons(u8 taskId)
{
    int i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0)
            && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) != SPECIES_NONE
            && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2) != SPECIES_EGG)
        {
            u32 curStatus = STATUS1_POISON;
            SetMonData(&gPlayerParty[i], MON_DATA_STATUS, &curStatus);
        }
    }
    PlaySE(SE_FIELD_POISON);
}

static void DebugAction_Util_CheckSaveBlock(u8 taskId)
{
    static const u8 sDebugText_SaveBlockSize[] =  _("SaveBlock1 is {STR_VAR_1} bytes long.\nMax size is 15872 bytes.\pSaveBlock2 is {STR_VAR_2} bytes long.\nMax size is 3968 bytes.\pPokemonStorage is {STR_VAR_3} bytes long.\nMax size is 35712 bytes.");

    ConvertIntToDecimalStringN(gStringVar1, sizeof(struct SaveBlock1), STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, sizeof(struct SaveBlock2), STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, sizeof(struct PokemonStorage), STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, sDebugText_SaveBlockSize);

    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_ShowFieldMessageStringVar4);
}

static const u8 sWeatherNames[22][24] = {
    [WEATHER_NONE]               = _("NONE"),
    [WEATHER_SUNNY_CLOUDS]       = _("SUNNY CLOUDS"),
    [WEATHER_SUNNY]              = _("SUNNY"),
    [WEATHER_RAIN]               = _("RAIN"),
    [WEATHER_SNOW]               = _("SNOW"),
    [WEATHER_RAIN_THUNDERSTORM]  = _("RAIN THUNDERSTORM"),
    [WEATHER_FOG_HORIZONTAL]     = _("FOG HORIZONTAL"),
    [WEATHER_VOLCANIC_ASH]       = _("VOLCANIC ASH"),
    [WEATHER_SANDSTORM]          = _("SANDSTORM"),
    [WEATHER_FOG_DIAGONAL]       = _("FOG DIAGONAL"),
    [WEATHER_UNDERWATER]         = _("UNDERWATER"),
    [WEATHER_SHADE]              = _("SHADE"),
    [WEATHER_DROUGHT]            = _("DROUGHT"),
    [WEATHER_DOWNPOUR]           = _("DOWNPOUR"),
    [WEATHER_UNDERWATER_BUBBLES] = _("UNDERWATER BUBBLES"),
    [WEATHER_ABNORMAL]           = _("ABNORMAL(NOT WORKING)"),
    [WEATHER_ROUTE119_CYCLE]     = _("ROUTE119 CYCLE"),
    [WEATHER_ROUTE123_CYCLE]     = _("ROUTE123 CYCLE"),
};
static const u8 sText_WeatherNotDefined[] = _("NOT DEFINED!!!");
static void DebugAction_Util_Weather(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayMediumWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar1, sWeatherNames[0], CHAR_SPACE, 30);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Weather_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 0;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
}
static void DebugAction_Util_Weather_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > WEATHER_ROUTE123_CYCLE)
                gTasks[taskId].data[3] = WEATHER_ROUTE123_CYCLE;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < WEATHER_NONE)
                gTasks[taskId].data[3] = WEATHER_NONE;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);

        if (gTasks[taskId].data[3] <= 15 || gTasks[taskId].data[3] >= 20)
            StringCopyPadded(gStringVar1, sWeatherNames[gTasks[taskId].data[3]], CHAR_SPACE, 30);
        else
            StringCopyPadded(gStringVar1, sText_WeatherNotDefined, CHAR_SPACE, 30); 

        StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].data[3] <= 14 || gTasks[taskId].data[3] >= 20)
        {
            gTasks[taskId].data[5] = gTasks[taskId].data[3];
            SetWeather(gTasks[taskId].data[5]);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_CheckWallClock(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(PlayersHouse_2F_EventScript_CheckWallClock);
}
static void DebugAction_Util_SetWallClock(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(PlayersHouse_2F_EventScript_SetWallClock);
}
static void DebugAction_Util_WatchCredits(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_StartCreditsSequence);
}
static void DebugAction_Util_Trainer_Name(u8 taskId)
{
    NewGameBirchSpeech_SetDefaultPlayerName(Random() % 20);
    DoNamingScreen(0, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}
static void DebugAction_Util_Trainer_Gender(u8 taskId)
{
    if (gSaveBlock2Ptr->playerGender == 0) // 0 Male, 1 Female
        gSaveBlock2Ptr->playerGender = 1;
    else
        gSaveBlock2Ptr->playerGender = 0;
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_Util_Trainer_Id(u8 taskId)
{
    u32 trainerId = ((Random() << 16) | Random());
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

// *******************************
// Actions Scripts
static void DebugAction_Util_Script_1(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_1);
}
static void DebugAction_Util_Script_2(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_2);
}
static void DebugAction_Util_Script_3(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_3);
}
static void DebugAction_Util_Script_4(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_4);
}
static void DebugAction_Util_Script_5(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_5);
}
static void DebugAction_Util_Script_6(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_6);
}
static void DebugAction_Util_Script_7(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_7);
}
static void DebugAction_Util_Script_8(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_Script_8);
}

// *******************************
// Actions Flags
static void DebugAction_Flags_Flags(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial Flag
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_FLAGS);
    ConvertIntToHexStringN(gStringVar2, 1, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar1, sDebugText_Flags_FlagHex);
    if (FlagGet(FLAG_TEMP_1) == TRUE)
        StringCopyPadded(gStringVar2, sDebugText_Flags_FlagSet, CHAR_SPACE, 15);
    else
        StringCopyPadded(gStringVar2, sDebugText_Flags_FlagUnset, CHAR_SPACE, 15);
    StringCopy(gStringVar3, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_Flags_Flag);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Flags_FlagsSelect;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = FLAG_TEMP_1; //Current Flag
    gTasks[taskId].data[4] = 0;           //Digit Selected
}
static void DebugAction_Flags_FlagsSelect(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
        FlagToggle(gTasks[taskId].data[3]);
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] >= FLAGS_COUNT){
            gTasks[taskId].data[3] = FLAGS_COUNT - 1;
        }
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] < 1){
            gTasks[taskId].data[3] = 1;
        }
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
        {
            gTasks[taskId].data[4] = 0;
        }
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > DEBUG_NUMBER_DIGITS_FLAGS - 1)
        {
            gTasks[taskId].data[4] = DEBUG_NUMBER_DIGITS_FLAGS - 1;
        }
    }

    if (JOY_NEW(DPAD_ANY) || JOY_NEW(A_BUTTON))
    {
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_FLAGS);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar1, sDebugText_Flags_FlagHex);
        if (FlagGet(gTasks[taskId].data[3]) == TRUE)
            StringCopyPadded(gStringVar2, sDebugText_Flags_FlagSet, CHAR_SPACE, 15);
        else
            StringCopyPadded(gStringVar2, sDebugText_Flags_FlagUnset, CHAR_SPACE, 15);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Flags_Flag);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }
}

static void DebugAction_Flags_SetPokedexFlags(u8 taskId)
{
    u16 i;
    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        GetSetPokedexFlag(i + 1, FLAG_SET_CAUGHT);
        GetSetPokedexFlag(i + 1, FLAG_SET_SEEN);
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}
static void DebugAction_Flags_SwitchDex(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKEDEX_GET);
}
static void DebugAction_Flags_SwitchNatDex(u8 taskId)
{
    if (IsNationalPokedexEnabled())
    {
        DisableNationalPokedex();
        PlaySE(SE_PC_OFF);
    }
    else
    {
        EnableNationalPokedex();
        PlaySE(SE_PC_LOGIN);
    }
}
static void DebugAction_Flags_SwitchPokeNav(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKENAV_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKENAV_GET);
}
static void DebugAction_Flags_ToggleFlyFlags(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_LANDMARK_BATTLE_FRONTIER))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);

    FlagToggle(FLAG_VISITED_LITTLEROOT_TOWN);
    FlagToggle(FLAG_VISITED_OLDALE_TOWN);
    FlagToggle(FLAG_VISITED_DEWFORD_TOWN);
    FlagToggle(FLAG_VISITED_LAVARIDGE_TOWN);
    FlagToggle(FLAG_VISITED_FALLARBOR_TOWN);
    FlagToggle(FLAG_VISITED_VERDANTURF_TOWN);
    FlagToggle(FLAG_VISITED_PACIFIDLOG_TOWN);
    FlagToggle(FLAG_VISITED_PETALBURG_CITY);
    FlagToggle(FLAG_VISITED_SLATEPORT_CITY);
    FlagToggle(FLAG_VISITED_MAUVILLE_CITY);
    FlagToggle(FLAG_VISITED_RUSTBORO_CITY);
    FlagToggle(FLAG_VISITED_FORTREE_CITY);
    FlagToggle(FLAG_VISITED_LILYCOVE_CITY);
    FlagToggle(FLAG_VISITED_MOSSDEEP_CITY);
    FlagToggle(FLAG_VISITED_SOOTOPOLIS_CITY);
    FlagToggle(FLAG_VISITED_EVER_GRANDE_CITY);
    FlagToggle(FLAG_LANDMARK_POKEMON_LEAGUE);
    FlagToggle(FLAG_LANDMARK_BATTLE_FRONTIER);
}
static void DebugAction_Flags_ToggleBadgeFlags(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_BADGE08_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);

    FlagToggle(FLAG_BADGE01_GET);
    FlagToggle(FLAG_BADGE02_GET);
    FlagToggle(FLAG_BADGE03_GET);
    FlagToggle(FLAG_BADGE04_GET);
    FlagToggle(FLAG_BADGE05_GET);
    FlagToggle(FLAG_BADGE06_GET);
    FlagToggle(FLAG_BADGE07_GET);
    FlagToggle(FLAG_BADGE08_GET);
}
static void DebugAction_Flags_ToggleFrontierPass(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_FRONTIER_PASS);
}
static void DebugAction_Flags_CollisionOnOff(u8 taskId)
{
#if DEBUG_FLAG_NO_COLLISION == 0
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_FlagsNotSetMessage);
#else
    if (FlagGet(DEBUG_FLAG_NO_COLLISION))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(DEBUG_FLAG_NO_COLLISION);
#endif
}
static void DebugAction_Flags_EncounterOnOff(u8 taskId)
{
#if OW_FLAG_NO_ENCOUNTER == 0
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_FlagsNotSetMessage);
#else
    if (FlagGet(OW_FLAG_NO_ENCOUNTER))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(OW_FLAG_NO_ENCOUNTER);
#endif
}
static void DebugAction_Flags_TrainerSeeOnOff(u8 taskId)
{
#if OW_FLAG_NO_TRAINER_SEE == 0
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_FlagsNotSetMessage);
#else
    if (FlagGet(OW_FLAG_NO_TRAINER_SEE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(OW_FLAG_NO_TRAINER_SEE);
#endif
}
static void DebugAction_Flags_BagUseOnOff(u8 taskId)
{
#if B_FLAG_NO_BAG_USE == 0
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_FlagsNotSetMessage);
#else
    if (FlagGet(B_FLAG_NO_BAG_USE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(B_FLAG_NO_BAG_USE);
#endif
}
static void DebugAction_Flags_CatchingOnOff(u8 taskId)
{
#if B_FLAG_NO_CATCHING_USE == 0
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_FlagsNotSetMessage);
#else
    if (FlagGet(B_FLAG_NO_CATCHING))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(B_FLAG_NO_CATCHING);
#endif
}

// *******************************
// Actions Variables
static void DebugAction_Vars_Vars(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial Variable
    ConvertIntToDecimalStringN(gStringVar1, VARS_START, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    ConvertIntToHexStringN(gStringVar2, VARS_START, STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar1, sDebugText_Vars_VariableHex);
    ConvertIntToDecimalStringN(gStringVar3, 0, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_Vars_Variable);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Vars_Select;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = VARS_START; //Current Variable
    gTasks[taskId].data[4] = 0;          //Digit Selected
    gTasks[taskId].data[5] = 0;            //Current Variable VALUE
}

static void DebugAction_Vars_Select(u8 taskId)
{
    if (JOY_NEW(DPAD_UP))
    {
        gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] > VARS_END)
            gTasks[taskId].data[3] = VARS_END;
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[3] < VARS_START)
            gTasks[taskId].data[3] = VARS_START;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
            gTasks[taskId].data[4] = 0;
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > DEBUG_NUMBER_DIGITS_VARIABLES - 1)
            gTasks[taskId].data[4] = DEBUG_NUMBER_DIGITS_VARIABLES - 1;
    }

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_Vars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].data[3]) == 65535) //Current value, if 65535 the value hasnt been set
            gTasks[taskId].data[5] = 0;
        else
            gTasks[taskId].data[5] = VarGet(gTasks[taskId].data[3]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[5], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit

        //Combine str's to full window string
        StringExpandPlaceholders(gStringVar4, sDebugText_Vars_Variable);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].data[4] = 0;

        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_Vars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].data[3]) == 65535) //Current value if 65535 the value hasnt been set
            gTasks[taskId].data[5] = 0;
        else
            gTasks[taskId].data[5] = VarGet(gTasks[taskId].data[3]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[5], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit
        StringExpandPlaceholders(gStringVar4, sDebugText_Vars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].data[6] = gTasks[taskId].data[5]; //New value selector
        gTasks[taskId].func = DebugAction_Vars_SetValue;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }
}
static void DebugAction_Vars_SetValue(u8 taskId)
{
    if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].data[6] + sPowersOfTen[gTasks[taskId].data[4]] <= 32000)
            gTasks[taskId].data[6] += sPowersOfTen[gTasks[taskId].data[4]];
        else
            gTasks[taskId].data[6] = 32000 - 1;

        if (gTasks[taskId].data[6] >= 32000)
            gTasks[taskId].data[6] = 32000 - 1;
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        gTasks[taskId].data[6] -= sPowersOfTen[gTasks[taskId].data[4]];
        if (gTasks[taskId].data[6] < 0){
            gTasks[taskId].data[6] = 0;
        }
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        gTasks[taskId].data[4] -= 1;
        if (gTasks[taskId].data[4] < 0)
        {
            gTasks[taskId].data[4] = 0;
        }
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        gTasks[taskId].data[4] += 1;
        if (gTasks[taskId].data[4] > 4)
        {
            gTasks[taskId].data[4] = 4;
        }
    }

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        VarSet(gTasks[taskId].data[3], gTasks[taskId].data[6]);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    if (JOY_NEW(DPAD_ANY) || JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].data[3], STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_Vars_VariableHex);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[6], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]); //Current digit
        StringExpandPlaceholders(gStringVar4, sDebugText_Vars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }
}


// *******************************
// Actions Give
#define ITEM_TAG 0xFDF3
static void DebugAction_Give_Item(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    CopyItemName(1, gStringVar1);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_ItemID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Give_Item_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[6] = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].data[3]);
    gSprites[gTasks[taskId].data[6]].x2 = DEBUG_NUMBER_ICON_X+10;
    gSprites[gTasks[taskId].data[6]].y2 = DEBUG_NUMBER_ICON_Y+10;
    gSprites[gTasks[taskId].data[6]].oam.priority = 0;
}
static void DebugAction_Give_Item_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= ITEMS_COUNT)
                gTasks[taskId].data[3] = ITEMS_COUNT - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS - 1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        CopyItemName(gTasks[taskId].data[3], gStringVar1);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon
        gTasks[taskId].data[6] = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].data[3]);
        gSprites[gTasks[taskId].data[6]].x2 = DEBUG_NUMBER_ICON_X+10;
        gSprites[gTasks[taskId].data[6]].y2 = DEBUG_NUMBER_ICON_Y+10;
        gSprites[gTasks[taskId].data[6]].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        gTasks[taskId].data[3] = 1;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEM_QUANTITY);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemQuantity);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Item_SelectQuantity;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Item_SelectQuantity(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= 100)
                gTasks[taskId].data[3] = 99;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEM_QUANTITY);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_ItemQuantity);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(MUS_OBTAIN_ITEM);
        AddBagItem(gTasks[taskId].data[5], gTasks[taskId].data[3]);
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        FreeSpriteTilesByTag(ITEM_TAG);                         //Destroy item icon
        FreeSpritePaletteByTag(ITEM_TAG);                       //Destroy item icon
        FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[6]]); //Destroy item icon
        DestroySprite(&gSprites[gTasks[taskId].data[6]]);       //Destroy item icon

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

//TMs
static void DebugAction_Give_AllTMs(u8 taskId)
{
    u16 i;
    PlayFanfare(MUS_OBTAIN_TMHM);
    for (i = ITEM_TM01; i <= ITEM_HM08; i++)
    {
        if (ItemIdToBattleMoveId(i) != MOVE_NONE && !CheckBagHasItem(i, 1))
            AddBagItem(i, 1);
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

//Pokemon
static void ResetMonDataStruct(struct DebugMonData *sDebugMonData)
{
    sDebugMonData->mon_speciesId    = 1;
    sDebugMonData->mon_level        = 1;
    sDebugMonData->isShiny          = 0;
    sDebugMonData->mon_natureId     = 0;
    sDebugMonData->mon_abilityNum   = 0;
    sDebugMonData->mon_iv_hp        = 0;
    sDebugMonData->mon_iv_atk       = 0;
    sDebugMonData->mon_iv_def       = 0;
    sDebugMonData->mon_iv_speed     = 0;
    sDebugMonData->mon_iv_satk      = 0;
    sDebugMonData->mon_iv_sdef      = 0;
}
static void DebugAction_Give_PokemonSimple(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringCopy(gStringVar1, gSpeciesNames[1]);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    //Set task data
    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 0;            //Complex?
    FreeMonIconPalettes();                 //Free space for new pallete
    LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
    gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
    gSprites[gTasks[taskId].data[6]].oam.priority = 0; //Mon Icon ID
}
static void DebugAction_Give_PokemonComplex(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 4);
    StringCopy(gStringVar1, gSpeciesNames[1]);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;            //Current ID
    gTasks[taskId].data[4] = 0;            //Digit Selected
    gTasks[taskId].data[5] = 1;            //Complex?
    FreeMonIconPalettes();                 //Free space for new palletes
    LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
    gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
    gSprites[gTasks[taskId].data[6]].oam.priority = 0; //Mon Icon ID
    gTasks[taskId].data[7] = 0;             //iterator
}

static void DebugAction_Give_Pokemon_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= NUM_SPECIES)
                gTasks[taskId].data[3] = NUM_SPECIES - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS - 1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopy(gStringVar1, gSpeciesNames[gTasks[taskId].data[3]]); //CopyItemName(gTasks[taskId].data[3], gStringVar1);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 4);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]);
        FreeMonIconPalettes(); //Free space for new pallete
        LoadMonIconPalette(gTasks[taskId].data[3]); //Loads pallete for current mon
        gTasks[taskId].data[6] = CreateMonIcon(gTasks[taskId].data[3], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0); //Create pokemon sprite
        gSprites[gTasks[taskId].data[6]].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->mon_speciesId = gTasks[taskId].data[3]; //Species ID
        gTasks[taskId].data[3] = 1;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonLevel);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectLevel;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 100)
                gTasks[taskId].data[3] = 100;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonLevel);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        if (gTasks[taskId].data[5] == 0)
        {
            PlaySE(MUS_LEVEL_UP);
            ScriptGiveMon(sDebugMonData->mon_speciesId, gTasks[taskId].data[3], ITEM_NONE, 0,0,0);
            //Set flag for user convenience
            FlagSet(FLAG_SYS_POKEMON_GET);
            Free(sDebugMonData); //Frees EWRAM of MonData Struct
            DebugAction_DestroyExtraWindow(taskId);
        }
        else
        {
            sDebugMonData->mon_level = gTasks[taskId].data[3]; //Level
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 0);
            StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
            StringCopyPadded(gStringVar2, sDebugText_Flags_FlagUnset, CHAR_SPACE, 15);
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonShiny);
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectShiny;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].data[6]]); //Destroy pokemon sprite
        DebugAction_DestroyExtraWindow(taskId);
    }
}
//If complex
static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        if (gTasks[taskId].data[3] == 1)
            StringCopyPadded(gStringVar2, sDebugText_Flags_FlagSet, CHAR_SPACE, 15);
        else
            StringCopyPadded(gStringVar2, sDebugText_Flags_FlagUnset, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 0);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonShiny);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->isShiny = gTasks[taskId].data[3]; //isShiny
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gNatureNamePointers[0]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonNature);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectNature;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectNature(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > NUM_NATURES-1)
                gTasks[taskId].data[3] = NUM_NATURES-1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gNatureNamePointers[gTasks[taskId].data[3]]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonNature);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        u8 abilityId;
        sDebugMonData->mon_natureId = gTasks[taskId].data[3]; //NatureId
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        abilityId = GetAbilityBySpecies(sDebugMonData->mon_speciesId, 0);
        StringCopy(gStringVar1, gAbilityNames[abilityId]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonAbility);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectAbility;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId)
{
    u8 abilityId;
    u8 abilityCount = NUM_ABILITY_SLOTS - 1; //-1 for proper iteration
    u8 i = 0;

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > abilityCount)
                gTasks[taskId].data[3] = abilityCount;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }

        while (GetAbilityBySpecies(sDebugMonData->mon_speciesId, gTasks[taskId].data[3] - i) == ABILITY_NONE && gTasks[taskId].data[3] - i < NUM_ABILITY_SLOTS)
        {
            i++;
        }
        abilityId = GetAbilityBySpecies(sDebugMonData->mon_speciesId, gTasks[taskId].data[3] - i);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar1, gAbilityNames[abilityId]);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonAbility);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->mon_abilityNum = gTasks[taskId].data[3] - i; //AbilityNum
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > 31)
                gTasks[taskId].data[3] = 31;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 2)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        switch (gTasks[taskId].data[7])
        {
        case 0:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
            break;
        case 1:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_1);
            break;
        case 2:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_2);
            break;
        case 3:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_3);
            break;
        case 4:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_4);
            break;
        case 5:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_5);
            break;
        }
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    //If A or B button
    if (JOY_NEW(A_BUTTON))
    {
        switch (gTasks[taskId].data[7])
        {
        case 0:
            sDebugMonData->mon_iv_hp = gTasks[taskId].data[3];
            break;
        case 1:
            sDebugMonData->mon_iv_atk = gTasks[taskId].data[3];
            break;
        case 2:
            sDebugMonData->mon_iv_def = gTasks[taskId].data[3];
            break;
        case 3:
            sDebugMonData->mon_iv_speed = gTasks[taskId].data[3];
            break;
        case 4:
            sDebugMonData->mon_iv_satk = gTasks[taskId].data[3];
            break;
        case 5:
            sDebugMonData->mon_iv_sdef = gTasks[taskId].data[3];
            break;
        }

        //Check if all IVs set
        if (gTasks[taskId].data[7] != 5)
        {
            gTasks[taskId].data[7] += 1;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 2);
            StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
            switch (gTasks[taskId].data[7])
            {
            case 0:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_0);
                break;
            case 1:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_1);
                break;
            case 2:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_2);
                break;
            case 3:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_3);
                break;
            case 4:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_4);
                break;
            case 5:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonIV_5);
                break;
            }
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
        }
        else
        {
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;
            gTasks[taskId].data[7] = 0; //Reset iterator

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
            StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_Move(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] >= MOVES_COUNT)
                gTasks[taskId].data[3] = MOVES_COUNT - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 0)
                gTasks[taskId].data[3] = 0;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < 3)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
        switch (gTasks[taskId].data[7])
        {
        case 0:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
            break;
        case 1:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_1);
            break;
        case 2:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_2);
            break;
        case 3:
            StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_3);
            break;
        }
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        //If MOVE_NONE selected, stop asking for additional moves
        if (gTasks[taskId].data[3] == 0)
            gTasks[taskId].data[7] = 4;

        //Set current value
        switch (gTasks[taskId].data[7])
        {
        case 0:
            sDebugMonData->mon_move_0 = gTasks[taskId].data[3];
            break;
        case 1:
            sDebugMonData->mon_move_1 = gTasks[taskId].data[3];
            break;
        case 2:
            sDebugMonData->mon_move_2 = gTasks[taskId].data[3];
            break;
        case 3:
            sDebugMonData->mon_move_3 = gTasks[taskId].data[3];
            break;
        }

        //If NOT last move or selected MOVE_NONE ask for next move, else make mon
        if (gTasks[taskId].data[7] < 3)
        {
            gTasks[taskId].data[7] += 1;
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
            StringCopy(gStringVar1, gMoveNames[gTasks[taskId].data[3]]);
            StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
            ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, 3);
            switch (gTasks[taskId].data[7])
            {
            case 0:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_0);
                break;
            case 1:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_1);
                break;
            case 2:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_2);
                break;
            case 3:
                StringExpandPlaceholders(gStringVar4, sDebugText_PokemonMove_3);
                break;
            }
            AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);

            gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
        }
        else
        {
            gTasks[taskId].data[3] = 0;
            gTasks[taskId].data[4] = 0;

            PlaySE(MUS_LEVEL_UP);
            gTasks[taskId].func = DebugAction_Give_Pokemon_ComplexCreateMon;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData); //Frees EWRAM of MonData Struct
        DebugAction_DestroyExtraWindow(taskId);
    }
}
static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId) //https://github.com/ghoulslash/pokeemerald/tree/custom-givemon
{
    u16 nationalDexNum;
    int sentToPc;
    struct Pokemon mon;
    u8 i;
    u16 moves[4];
    u8 IVs[6];
    u8 iv_val;
    u16 species     = sDebugMonData->mon_speciesId;
    u8 level        = sDebugMonData->mon_level;
    u8 isShiny      = sDebugMonData->isShiny; //Shiny: no 0, yes 1
    u8 nature       = sDebugMonData->mon_natureId;
    u8 abilityNum   = sDebugMonData->mon_abilityNum;
    moves[0]        = sDebugMonData->mon_move_0;
    moves[1]        = sDebugMonData->mon_move_1;
    moves[2]        = sDebugMonData->mon_move_2;
    moves[3]        = sDebugMonData->mon_move_3;
    IVs[0]          = sDebugMonData->mon_iv_hp;
    IVs[1]          = sDebugMonData->mon_iv_atk;
    IVs[2]          = sDebugMonData->mon_iv_def;
    IVs[3]          = sDebugMonData->mon_iv_speed;
    IVs[4]          = sDebugMonData->mon_iv_satk;
    IVs[5]          = sDebugMonData->mon_iv_sdef;

    //Nature
    if (nature == NUM_NATURES || nature == 0xFF)
        nature = Random() % NUM_NATURES;

    //Shininess
    if (isShiny == 1)
    {
        u32 personality;
        u32 otid = gSaveBlock2Ptr->playerTrainerId[0]
            | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
            | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
            | (gSaveBlock2Ptr->playerTrainerId[3] << 24);

        do
        {
            personality = Random32();
            personality = ((((Random() % 8) ^ (HIHALF(otid) ^ LOHALF(otid))) ^ LOHALF(personality)) << 16) | LOHALF(personality);
        } while (nature != GetNatureFromPersonality(personality));

        CreateMon(&mon, species, level, 32, 1, personality, OT_ID_PRESET, otid);
    }
    else
        CreateMonWithNature(&mon, species, level, 32, nature);

    //IVs
    for (i = 0; i < NUM_STATS; i++)
    {
        iv_val = IVs[i];
        if (iv_val != 32 && iv_val != 0xFF)
            SetMonData(&mon, MON_DATA_HP_IV + i, &iv_val);
    }
    CalculateMonStats(&mon);

    //Moves
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (moves[i] == 0 || moves[i] == 0xFF || moves[i] >= MOVES_COUNT)
            continue;

        SetMonMoveSlot(&mon, moves[i], i);
    }

    //Ability
    if (abilityNum == 0xFF || GetAbilityBySpecies(species, abilityNum) == 0)
    {
        do {
            abilityNum = Random() % 3;  // includes hidden abilities
        } while (GetAbilityBySpecies(species, abilityNum) == 0);
    }

    SetMonData(&mon, MON_DATA_ABILITY_NUM, &abilityNum);

    // give player the mon
    SetMonData(&mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    SetMonData(&mon, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }

    if (i >= PARTY_SIZE)
        sentToPc = SendMonToPC(&mon);
    else
    {
        sentToPc = MON_GIVEN_TO_PARTY;
        CopyMon(&gPlayerParty[i], &mon, sizeof(mon));
        gPlayerPartyCount = i + 1;
    }

    //Pokedex entry
    nationalDexNum = SpeciesToNationalPokedexNum(species);
    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    case MON_CANT_GIVE:
        break;
    }

    //Set flag for user convenience
    FlagSet(FLAG_SYS_POKEMON_GET);

    Free(sDebugMonData); //Frees EWRAM of MonData Struct
    DebugAction_DestroyExtraWindow(taskId); //return sentToPc;
}

static void DebugAction_Give_MaxMoney(u8 taskId)
{
    SetMoney(&gSaveBlock1Ptr->money, 999999);
}

static void DebugAction_Give_MaxCoins(u8 taskId)
{
    SetCoins(9999);
}

static void DebugAction_Give_MaxBattlePoints(u8 taskId)
{
    gSaveBlock2Ptr->frontier.battlePoints = MAX_BATTLE_FRONTIER_POINTS;
}

static void DebugAction_Give_DayCareEgg(u8 taskId)
{
    TriggerPendingDaycareEgg();
}

static void DebugAction_Give_FillPC(u8 taskId) //Credit: Sierraffinity
{
    int boxId, boxPosition;
    u32 personality;
    struct BoxPokemon boxMon;

    personality = Random32();

    CreateBoxMon(&boxMon,
                 SPECIES_DEOXYS,
                 100,
                 32,
                 personality,
                 0,
                 OT_ID_PLAYER_ID,
                 0);

    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (!GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                gPokemonStoragePtr->boxes[boxId][boxPosition] = boxMon;
            }
        }
    }
}

static void DebugAction_Give_CHEAT(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(Debug_CheatStart);
}

static void DebugAction_AccessPC(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    ScriptContext_SetupScript(EventScript_PC);
}


// *******************************
// Sound Scripts
static const u8 *const sBGMNames[];
static const u8 *const sSENames[];
static void DebugAction_Sound_SE(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayLargeWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, sSENames[0], CHAR_SPACE, 50);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SE_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    StopMapMusic(); //Stop map music to better hear sounds

    gTasks[taskId].func = DebugAction_Sound_SE_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = 1;                         //Current ID
    gTasks[taskId].data[4] = 0;                         //Digit Selected
    gTasks[taskId].data[5] = gTasks[taskId].data[3];    //Last song played (for stopping)
}
static void DebugAction_Sound_SE_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > END_SE)
                gTasks[taskId].data[3] = END_SE;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < 1)
                gTasks[taskId].data[3] = 1;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS - 1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopyPadded(gStringVar1, sSENames[gTasks[taskId].data[3]-1], CHAR_SPACE, 50);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SE_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].data[5]);
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        m4aSongNumStart(gTasks[taskId].data[3]);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        m4aSongNumStop(gTasks[taskId].data[5]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Sound_MUS(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].data[1], TRUE);
    RemoveWindow(gTasks[taskId].data[1]);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugNumberDisplayLargeWindowTemplate);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, 3);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, START_MUS, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, sBGMNames[0], CHAR_SPACE, 50);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_MUS_ID);
    AddTextPrinterParameterized(windowId, 1, gStringVar4, 1, 1, 0, NULL);

    StopMapMusic(); //Stop map music to better hear new music

    gTasks[taskId].func = DebugAction_Sound_MUS_SelectId;
    gTasks[taskId].data[2] = windowId;
    gTasks[taskId].data[3] = START_MUS;                 //Current ID
    gTasks[taskId].data[4] = 0;                         //Digit Selected
    gTasks[taskId].data[5] = gTasks[taskId].data[3];    //Last song played (for stopping)
}
static void DebugAction_Sound_MUS_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].data[3] += sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] > END_MUS)
                gTasks[taskId].data[3] = END_MUS;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].data[3] -= sPowersOfTen[gTasks[taskId].data[4]];
            if (gTasks[taskId].data[3] < START_MUS)
                gTasks[taskId].data[3] = START_MUS;
        }
        if (JOY_NEW(DPAD_LEFT))
        {
            if (gTasks[taskId].data[4] > 0)
                gTasks[taskId].data[4] -= 1;
        }
        if (JOY_NEW(DPAD_RIGHT))
        {
            if (gTasks[taskId].data[4] < DEBUG_NUMBER_DIGITS_ITEMS - 1)
                gTasks[taskId].data[4] += 1;
        }

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].data[4]]);
        StringCopyPadded(gStringVar1, sBGMNames[gTasks[taskId].data[3]-START_MUS], CHAR_SPACE, 50);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[3], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_MUS_ID);
        AddTextPrinterParameterized(gTasks[taskId].data[2], 1, gStringVar4, 1, 1, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].data[5]);
        gTasks[taskId].data[5] = gTasks[taskId].data[3];
        m4aSongNumStart(gTasks[taskId].data[3]);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        // m4aSongNumStop(gTasks[taskId].data[5]);   //Uncomment if music should stop after leaving menu
        DebugAction_DestroyExtraWindow(taskId);
    }
}

#define SOUND_LIST_BGM \
    X(MUS_LITTLEROOT_TEST) \
    X(MUS_GSC_ROUTE38) \
    X(MUS_CAUGHT) \
    X(MUS_VICTORY_WILD) \
    X(MUS_VICTORY_GYM_LEADER) \
    X(MUS_VICTORY_LEAGUE) \
    X(MUS_C_COMM_CENTER) \
    X(MUS_GSC_PEWTER) \
    X(MUS_C_VS_LEGEND_BEAST) \
    X(MUS_ROUTE101) \
    X(MUS_ROUTE110) \
    X(MUS_ROUTE120) \
    X(MUS_PETALBURG) \
    X(MUS_OLDALE) \
    X(MUS_GYM) \
    X(MUS_SURF) \
    X(MUS_PETALBURG_WOODS) \
    X(MUS_LEVEL_UP) \
    X(MUS_HEAL) \
    X(MUS_OBTAIN_BADGE) \
    X(MUS_OBTAIN_ITEM) \
    X(MUS_EVOLVED) \
    X(MUS_OBTAIN_TMHM) \
    X(MUS_LILYCOVE_MUSEUM) \
    X(MUS_ROUTE122) \
    X(MUS_OCEANIC_MUSEUM) \
    X(MUS_EVOLUTION_INTRO) \
    X(MUS_EVOLUTION) \
    X(MUS_MOVE_DELETED) \
    X(MUS_ENCOUNTER_GIRL) \
    X(MUS_ENCOUNTER_MALE) \
    X(MUS_ABANDONED_SHIP) \
    X(MUS_FORTREE) \
    X(MUS_BIRCH_LAB) \
    X(MUS_B_TOWER_RS) \
    X(MUS_ENCOUNTER_SWIMMER) \
    X(MUS_CAVE_OF_ORIGIN) \
    X(MUS_OBTAIN_BERRY) \
    X(MUS_AWAKEN_LEGEND) \
    X(MUS_SLOTS_JACKPOT) \
    X(MUS_SLOTS_WIN) \
    X(MUS_TOO_BAD) \
    X(MUS_ROULETTE) \
    X(MUS_LINK_CONTEST_P1) \
    X(MUS_LINK_CONTEST_P2) \
    X(MUS_LINK_CONTEST_P3) \
    X(MUS_LINK_CONTEST_P4) \
    X(MUS_ENCOUNTER_RICH) \
    X(MUS_VERDANTURF) \
    X(MUS_RUSTBORO) \
    X(MUS_POKE_CENTER) \
    X(MUS_ROUTE104) \
    X(MUS_ROUTE119) \
    X(MUS_CYCLING) \
    X(MUS_POKE_MART) \
    X(MUS_LITTLEROOT) \
    X(MUS_MT_CHIMNEY) \
    X(MUS_ENCOUNTER_FEMALE) \
    X(MUS_LILYCOVE) \
    X(MUS_ROUTE111) \
    X(MUS_HELP) \
    X(MUS_UNDERWATER) \
    X(MUS_VICTORY_TRAINER) \
    X(MUS_TITLE) \
    X(MUS_INTRO) \
    X(MUS_ENCOUNTER_MAY) \
    X(MUS_ENCOUNTER_INTENSE) \
    X(MUS_ENCOUNTER_COOL) \
    X(MUS_ROUTE113) \
    X(MUS_ENCOUNTER_AQUA) \
    X(MUS_FOLLOW_ME) \
    X(MUS_ENCOUNTER_BRENDAN) \
    X(MUS_EVER_GRANDE) \
    X(MUS_ENCOUNTER_SUSPICIOUS) \
    X(MUS_VICTORY_AQUA_MAGMA) \
    X(MUS_CABLE_CAR) \
    X(MUS_GAME_CORNER) \
    X(MUS_DEWFORD) \
    X(MUS_SAFARI_ZONE) \
    X(MUS_VICTORY_ROAD) \
    X(MUS_AQUA_MAGMA_HIDEOUT) \
    X(MUS_SAILING) \
    X(MUS_MT_PYRE) \
    X(MUS_SLATEPORT) \
    X(MUS_MT_PYRE_EXTERIOR) \
    X(MUS_SCHOOL) \
    X(MUS_HALL_OF_FAME) \
    X(MUS_FALLARBOR) \
    X(MUS_SEALED_CHAMBER) \
    X(MUS_CONTEST_WINNER) \
    X(MUS_CONTEST) \
    X(MUS_ENCOUNTER_MAGMA) \
    X(MUS_INTRO_BATTLE) \
    X(MUS_WEATHER_KYOGRE) \
    X(MUS_WEATHER_GROUDON) \
    X(MUS_SOOTOPOLIS) \
    X(MUS_CONTEST_RESULTS) \
    X(MUS_HALL_OF_FAME_ROOM) \
    X(MUS_TRICK_HOUSE) \
    X(MUS_ENCOUNTER_TWINS) \
    X(MUS_ENCOUNTER_ELITE_FOUR) \
    X(MUS_ENCOUNTER_HIKER) \
    X(MUS_CONTEST_LOBBY) \
    X(MUS_ENCOUNTER_INTERVIEWER) \
    X(MUS_ENCOUNTER_CHAMPION) \
    X(MUS_CREDITS) \
    X(MUS_END) \
    X(MUS_B_FRONTIER) \
    X(MUS_B_ARENA) \
    X(MUS_OBTAIN_B_POINTS) \
    X(MUS_REGISTER_MATCH_CALL) \
    X(MUS_B_PYRAMID) \
    X(MUS_B_PYRAMID_TOP) \
    X(MUS_B_PALACE) \
    X(MUS_RAYQUAZA_APPEARS) \
    X(MUS_B_TOWER) \
    X(MUS_OBTAIN_SYMBOL) \
    X(MUS_B_DOME) \
    X(MUS_B_PIKE) \
    X(MUS_B_FACTORY) \
    X(MUS_VS_RAYQUAZA) \
    X(MUS_VS_FRONTIER_BRAIN) \
    X(MUS_VS_MEW) \
    X(MUS_B_DOME_LOBBY) \
    X(MUS_VS_WILD) \
    X(MUS_VS_AQUA_MAGMA) \
    X(MUS_VS_TRAINER) \
    X(MUS_VS_GYM_LEADER) \
    X(MUS_VS_CHAMPION) \
    X(MUS_VS_REGI) \
    X(MUS_VS_KYOGRE_GROUDON) \
    X(MUS_VS_RIVAL) \
    X(MUS_VS_ELITE_FOUR) \
    X(MUS_VS_AQUA_MAGMA_LEADER) \
    X(MUS_RG_FOLLOW_ME) \
    X(MUS_RG_GAME_CORNER) \
    X(MUS_RG_ROCKET_HIDEOUT) \
    X(MUS_RG_GYM) \
    X(MUS_RG_JIGGLYPUFF) \
    X(MUS_RG_INTRO_FIGHT) \
    X(MUS_RG_TITLE) \
    X(MUS_RG_CINNABAR) \
    X(MUS_RG_LAVENDER) \
    X(MUS_RG_HEAL) \
    X(MUS_RG_CYCLING) \
    X(MUS_RG_ENCOUNTER_ROCKET) \
    X(MUS_RG_ENCOUNTER_GIRL) \
    X(MUS_RG_ENCOUNTER_BOY) \
    X(MUS_RG_HALL_OF_FAME) \
    X(MUS_RG_VIRIDIAN_FOREST) \
    X(MUS_RG_MT_MOON) \
    X(MUS_RG_POKE_MANSION) \
    X(MUS_RG_CREDITS) \
    X(MUS_RG_ROUTE1) \
    X(MUS_RG_ROUTE24) \
    X(MUS_RG_ROUTE3) \
    X(MUS_RG_ROUTE11) \
    X(MUS_RG_VICTORY_ROAD) \
    X(MUS_RG_VS_GYM_LEADER) \
    X(MUS_RG_VS_TRAINER) \
    X(MUS_RG_VS_WILD) \
    X(MUS_RG_VS_CHAMPION) \
    X(MUS_RG_PALLET) \
    X(MUS_RG_OAK_LAB) \
    X(MUS_RG_OAK) \
    X(MUS_RG_POKE_CENTER) \
    X(MUS_RG_SS_ANNE) \
    X(MUS_RG_SURF) \
    X(MUS_RG_POKE_TOWER) \
    X(MUS_RG_SILPH) \
    X(MUS_RG_FUCHSIA) \
    X(MUS_RG_CELADON) \
    X(MUS_RG_VICTORY_TRAINER) \
    X(MUS_RG_VICTORY_WILD) \
    X(MUS_RG_VICTORY_GYM_LEADER) \
    X(MUS_RG_VERMILLION) \
    X(MUS_RG_PEWTER) \
    X(MUS_RG_ENCOUNTER_RIVAL) \
    X(MUS_RG_RIVAL_EXIT) \
    X(MUS_RG_DEX_RATING) \
    X(MUS_RG_OBTAIN_KEY_ITEM) \
    X(MUS_RG_CAUGHT_INTRO) \
    X(MUS_RG_PHOTO) \
    X(MUS_RG_GAME_FREAK) \
    X(MUS_RG_CAUGHT) \
    X(MUS_RG_NEW_GAME_INSTRUCT) \
    X(MUS_RG_NEW_GAME_INTRO) \
    X(MUS_RG_NEW_GAME_EXIT) \
    X(MUS_RG_POKE_JUMP) \
    X(MUS_RG_UNION_ROOM) \
    X(MUS_RG_NET_CENTER) \
    X(MUS_RG_MYSTERY_GIFT) \
    X(MUS_RG_BERRY_PICK) \
    X(MUS_RG_SEVII_CAVE) \
    X(MUS_RG_TEACHY_TV_SHOW) \
    X(MUS_RG_SEVII_ROUTE) \
    X(MUS_RG_SEVII_DUNGEON) \
    X(MUS_RG_SEVII_123) \
    X(MUS_RG_SEVII_45) \
    X(MUS_RG_SEVII_67) \
    X(MUS_RG_POKE_FLUTE) \
    X(MUS_RG_VS_DEOXYS) \
    X(MUS_RG_VS_MEWTWO) \
    X(MUS_RG_VS_LEGEND) \
    X(MUS_RG_ENCOUNTER_GYM_LEADER) \
    X(MUS_RG_ENCOUNTER_DEOXYS) \
    X(MUS_RG_TRAINER_TOWER) \
    X(MUS_RG_SLOW_PALLET) \
    X(MUS_RG_TEACHY_TV_MENU) \
    X(PH_TRAP_BLEND) \
    X(PH_TRAP_HELD) \
    X(PH_TRAP_SOLO) \
    X(PH_FACE_BLEND) \
    X(PH_FACE_HELD) \
    X(PH_FACE_SOLO) \
    X(PH_CLOTH_BLEND) \
    X(PH_CLOTH_HELD) \
    X(PH_CLOTH_SOLO) \
    X(PH_DRESS_BLEND) \
    X(PH_DRESS_HELD) \
    X(PH_DRESS_SOLO) \
    X(PH_FLEECE_BLEND) \
    X(PH_FLEECE_HELD) \
    X(PH_FLEECE_SOLO) \
    X(PH_KIT_BLEND) \
    X(PH_KIT_HELD) \
    X(PH_KIT_SOLO) \
    X(PH_PRICE_BLEND) \
    X(PH_PRICE_HELD) \
    X(PH_PRICE_SOLO) \
    X(PH_LOT_BLEND) \
    X(PH_LOT_HELD) \
    X(PH_LOT_SOLO) \
    X(PH_GOAT_BLEND) \
    X(PH_GOAT_HELD) \
    X(PH_GOAT_SOLO) \
    X(PH_THOUGHT_BLEND) \
    X(PH_THOUGHT_HELD) \
    X(PH_THOUGHT_SOLO) \
    X(PH_CHOICE_BLEND) \
    X(PH_CHOICE_HELD) \
    X(PH_CHOICE_SOLO) \
    X(PH_MOUTH_BLEND) \
    X(PH_MOUTH_HELD) \
    X(PH_MOUTH_SOLO) \
    X(PH_FOOT_BLEND) \
    X(PH_FOOT_HELD) \
    X(PH_FOOT_SOLO) \
    X(PH_GOOSE_BLEND) \
    X(PH_GOOSE_HELD) \
    X(PH_GOOSE_SOLO) \
    X(PH_STRUT_BLEND) \
    X(PH_STRUT_HELD) \
    X(PH_STRUT_SOLO) \
    X(PH_CURE_BLEND) \
    X(PH_CURE_HELD) \
    X(PH_CURE_SOLO) \
    X(PH_NURSE_BLEND) \
    X(PH_NURSE_HELD) \
    X(PH_NURSE_SOLO) \
	X(MUS_HG_INTRO) \
	X(MUS_HG_TITLE) \
	X(MUS_HG_NEW_GAME) \
	X(MUS_HG_EVOLUTION) \
	X(MUS_HG_EVOLUTION_NO_INTRO) \
	X(MUS_HG_CYCLING) \
	X(MUS_HG_SURF) \
	X(MUS_HG_E_DENDOURIRI) \
	X(MUS_HG_CREDITS) \
	X(MUS_HG_END) \
	X(MUS_HG_NEW_BARK) \
	X(MUS_HG_CHERRYGROVE) \
	X(MUS_HG_VIOLET) \
	X(MUS_HG_AZALEA) \
	X(MUS_HG_GOLDENROD) \
	X(MUS_HG_ECRUTEAK) \
	X(MUS_HG_CIANWOOD) \
	X(MUS_HG_ROUTE29) \
	X(MUS_HG_ROUTE30) \
	X(MUS_HG_ROUTE34) \
	X(MUS_HG_ROUTE38) \
	X(MUS_HG_ROUTE42) \
	X(MUS_HG_VERMILION) \
	X(MUS_HG_PEWTER) \
	X(MUS_HG_CERULEAN) \
	X(MUS_HG_LAVENDER) \
	X(MUS_HG_CELADON) \
	X(MUS_HG_PALLET) \
	X(MUS_HG_CINNABAR) \
	X(MUS_HG_ROUTE1) \
	X(MUS_HG_ROUTE3) \
	X(MUS_HG_ROUTE11) \
	X(MUS_HG_ROUTE24) \
	X(MUS_HG_ROUTE26) \
	X(MUS_HG_POKE_CENTER) \
	X(MUS_HG_POKE_MART) \
	X(MUS_HG_GYM) \
	X(MUS_HG_ELM_LAB) \
	X(MUS_HG_OAK) \
	X(MUS_HG_DANCE_THEATER) \
	X(MUS_HG_GAME_CORNER) \
	X(MUS_HG_B_TOWER) \
	X(MUS_HG_B_TOWER_RECEPTION) \
	X(MUS_HG_SPROUT_TOWER) \
	X(MUS_HG_UNION_CAVE) \
	X(MUS_HG_RUINS_OF_ALPH) \
	X(MUS_HG_NATIONAL_PARK) \
	X(MUS_HG_BURNED_TOWER) \
	X(MUS_HG_BELL_TOWER) \
	X(MUS_HG_LIGHTHOUSE) \
	X(MUS_HG_TEAM_ROCKET_HQ) \
	X(MUS_HG_ICE_PATH) \
	X(MUS_HG_DRAGONS_DEN) \
	X(MUS_HG_ROCK_TUNNEL) \
	X(MUS_HG_VIRIDIAN_FOREST) \
	X(MUS_HG_VICTORY_ROAD) \
	X(MUS_HG_POKEMON_LEAGUE) \
	X(MUS_HG_FOLLOW_ME_1) \
	X(MUS_HG_FOLLOW_ME_2) \
	X(MUS_HG_ENCOUNTER_RIVAL) \
	X(MUS_HG_RIVAL_EXIT) \
	X(MUS_HG_BUG_CONTEST_PREP) \
	X(MUS_HG_BUG_CATCHING_CONTEST) \
	X(MUS_HG_RADIO_ROCKET) \
	X(MUS_HG_ROCKET_TAKEOVER) \
	X(MUS_HG_MAGNET_TRAIN) \
	X(MUS_HG_SS_AQUA) \
	X(MUS_HG_MT_MOON_SQUARE) \
	X(MUS_HG_RADIO_JINGLE) \
	X(MUS_HG_RADIO_LULLABY) \
	X(MUS_HG_RADIO_MARCH) \
	X(MUS_HG_RADIO_UNOWN) \
	X(MUS_HG_RADIO_POKE_FLUTE) \
	X(MUS_HG_RADIO_OAK) \
	X(MUS_HG_RADIO_BUENA) \
	X(MUS_HG_EUSINE) \
	X(MUS_HG_CLAIR) \
	X(MUS_HG_ENCOUNTER_GIRL_1) \
	X(MUS_HG_ENCOUNTER_BOY_1) \
	X(MUS_HG_ENCOUNTER_SUSPICIOUS_1) \
	X(MUS_HG_ENCOUNTER_SAGE) \
	X(MUS_HG_ENCOUNTER_KIMONO_GIRL) \
	X(MUS_HG_ENCOUNTER_ROCKET) \
	X(MUS_HG_ENCOUNTER_GIRL_2) \
	X(MUS_HG_ENCOUNTER_BOY_2) \
	X(MUS_HG_ENCOUNTER_SUSPICIOUS_2) \
	X(MUS_HG_VS_WILD) \
	X(MUS_HG_VS_TRAINER) \
	X(MUS_HG_VS_GYM_LEADER) \
	X(MUS_HG_VS_RIVAL) \
	X(MUS_HG_VS_ROCKET) \
	X(MUS_HG_VS_SUICUNE) \
	X(MUS_HG_VS_ENTEI) \
	X(MUS_HG_VS_RAIKOU) \
	X(MUS_HG_VS_CHAMPION) \
	X(MUS_HG_VS_WILD_KANTO) \
	X(MUS_HG_VS_TRAINER_KANTO) \
	X(MUS_HG_VS_GYM_LEADER_KANTO) \
	X(MUS_HG_VICTORY_TRAINER) \
	X(MUS_HG_VICTORY_WILD) \
	X(MUS_HG_CAUGHT) \
	X(MUS_HG_VICTORY_GYM_LEADER) \
	X(MUS_HG_VS_HO_OH) \
	X(MUS_HG_VS_LUGIA) \
	X(MUS_HG_POKEATHLON_LOBBY) \
	X(MUS_HG_POKEATHLON_START) \
	X(MUS_HG_POKEATHLON_BEFORE) \
	X(MUS_HG_POKEATHLON_EVENT) \
	X(MUS_HG_POKEATHLON_FINALS) \
	X(MUS_HG_POKEATHLON_RESULTS) \
	X(MUS_HG_POKEATHLON_END) \
	X(MUS_HG_POKEATHLON_WINNER) \
	X(MUS_HG_B_FACTORY) \
	X(MUS_HG_B_HALL) \
	X(MUS_HG_B_ARCADE) \
	X(MUS_HG_B_CASTLE) \
	X(MUS_HG_VS_FRONTIER_BRAIN) \
	X(MUS_HG_VICTORY_FRONTIER_BRAIN) \
	X(MUS_HG_WFC) \
	X(MUS_HG_MYSTERY_GIFT) \
	X(MUS_HG_WIFI_PLAZA) \
	X(MUS_HG_WIFI_MINIGAMES) \
	X(MUS_HG_WIFI_PARADE) \
	X(MUS_HG_GLOBAL_TERMINAL) \
	X(MUS_HG_SPIN_TRADE) \
	X(MUS_HG_GTS) \
	X(MUS_HG_ROUTE47) \
	X(MUS_HG_SAFARI_ZONE_GATE) \
	X(MUS_HG_SAFARI_ZONE) \
	X(MUS_HG_ETHAN) \
	X(MUS_HG_LYRA) \
	X(MUS_HG_GAME_CORNER_WIN) \
	X(MUS_HG_KIMONO_GIRL_DANCE) \
	X(MUS_HG_KIMONO_GIRL) \
	X(MUS_HG_HO_OH_APPEARS) \
	X(MUS_HG_LUGIA_APPEARS) \
	X(MUS_HG_SPIKY_EARED_PICHU) \
	X(MUS_HG_SINJOU_RUINS) \
	X(MUS_HG_RADIO_ROUTE101) \
	X(MUS_HG_RADIO_ROUTE201) \
	X(MUS_HG_RADIO_TRAINER) \
	X(MUS_HG_RADIO_VARIETY) \
	X(MUS_HG_VS_KYOGRE_GROUDON) \
	X(MUS_HG_POKEWALKER) \
	X(MUS_HG_VS_ARCEUS) \
	X(MUS_HG_HEAL) \
	X(MUS_HG_LEVEL_UP) \
	X(MUS_HG_OBTAIN_ITEM) \
	X(MUS_HG_OBTAIN_KEY_ITEM) \
	X(MUS_HG_EVOLVED) \
	X(MUS_HG_OBTAIN_BADGE) \
	X(MUS_HG_OBTAIN_TMHM) \
	X(MUS_HG_OBTAIN_ACCESSORY) \
	X(MUS_HG_MOVE_DELETED) \
	X(MUS_HG_OBTAIN_BERRY) \
	X(MUS_HG_DEX_RATING_1) \
	X(MUS_HG_DEX_RATING_2) \
	X(MUS_HG_DEX_RATING_3) \
	X(MUS_HG_DEX_RATING_4) \
	X(MUS_HG_DEX_RATING_5) \
	X(MUS_HG_DEX_RATING_6) \
	X(MUS_HG_OBTAIN_EGG) \
	X(MUS_HG_BUG_CONTEST_1ST_PLACE) \
	X(MUS_HG_BUG_CONTEST_2ND_PLACE) \
	X(MUS_HG_BUG_CONTEST_3RD_PLACE) \
	X(MUS_HG_CARD_FLIP) \
	X(MUS_HG_CARD_FLIP_GAME_OVER) \
	X(MUS_HG_POKEGEAR_REGISTERED) \
	X(MUS_HG_LETS_GO_TOGETHER) \
	X(MUS_HG_POKEATHLON_READY) \
	X(MUS_HG_POKEATHLON_1ST_PLACE) \
	X(MUS_HG_RECEIVE_POKEMON) \
	X(MUS_HG_OBTAIN_ARCADE_POINTS) \
	X(MUS_HG_OBTAIN_CASTLE_POINTS) \
	X(MUS_HG_OBTAIN_B_POINTS) \
	X(MUS_HG_WIN_MINIGAME) \
	X(MUS_YYYI_AURORADAYS) \
	X(MUS_YYYI_HOSHITOHANA) \
	X(MUS_PC_ULTRACUREHAPPYTANJOU) \
	X(MUS_DCD_JTDORCHESTRA) \
	X(MUS_TH_SKYHIGH) \
	X(MUS_SH_PANDORA) \
	X(MUS_DGMN_KIZUNA) \
	X(MUS_PC_ULTRACUREHAPPYTANJOU_NOLOOP) \
	X(MUS_YYYI_SAZANKA) \
	X(MUS_PKMN_SV_TERARAIDBATTLE) \
	X(MUS_PC_DANZEN) \
	X(MUS_PC_OPENMYHEART) \
	X(MUS_GS_GS1ALLEGRO) \
	X(MUS_THPPZGSK_PRIMALSCENE) \
	X(MUS_THPPZGSK_PLASTICMIND) \
	X(MUS_THPPZGSK_FATEOFSIXTYYEARS_B) \
	X(MUS_THPPZGSK_DREAMBATTLE) \
	X(MUS_THPPZGSK_MASTERSPARK_A) \
	X(MUS_THPPZGSK_MASTERSPARK_B) \
	X(MUS_THPPZGSK_FAITH) \
	X(MUS_THPPZGSK_SUWAFOUGHTENFIELD) \
	X(MUS_THPPZGSK_MISSINGPOWER) \
	X(MUS_THPPZGSK_BORDEROFLIFE) \
	X(MUS_THPPZGSK_NUCLEARFUSION) \
	X(MUS_THPPZGSK_DREAMEXPRESS) \
	X(MUS_THPPZGSK_FAIRYWARS) \
	X(MUS_THPPZGSK_MAGUSNIGHT) \
	X(MUS_THPPZGSK_WITCHBALL) \
	X(MUS_THPPZGSK_WINDGODGIRL) \
	X(MUS_THPPZGSK_MYSTERIOUSMOUNTAIN) \
	X(MUS_THPPZGSK_LUNADIAL) \
	X(MUS_THPPZGSK_LUNATICPRINCESS) \
	X(MUS_THPPZGSK_CANDIDFRIEND) \
	X(MUS_THPPZGSK_PHANTOMENSEMBLE_A) \
	X(MUS_THPPZGSK_PHANTOMENSEMBLE_B) \
	X(MUS_THPPZGSK_388) \
	X(MUS_THPPZGSK_ABSORBEDCURIOUSITY) \
	X(MUS_THPPZGSK_AGODTHATLOVESPEOPLE) \
	X(MUS_THPPZGSK_ALICEINWONDERLAND) \
	X(MUS_THPPZGSK_DARKBLOWHOLE) \
	X(MUS_THPPZGSK_DEEPMOUNTAIN) \
	X(MUS_THPPZGSK_DREAMLAND) \
	X(MUS_THPPZGSK_ENDOFSPRING) \
	X(MUS_THPPZGSK_FALLOFFALL) \
	X(MUS_THPPZGSK_FATEOFSIXTYYEARS_A) \
	X(MUS_THPPZGSK_FIRESOFHOKKAI) \
	X(MUS_THPPZGSK_FLOWERLAND) \
	X(MUS_THPPZGSK_FLOWERSREMAININFANTASY) \
	X(MUS_THPPZGSK_GHOSTLYEYES) \
	X(MUS_THPPZGSK_JAPANESEFLOWER) \
	X(MUS_THPPZGSK_LOCKEDGIRL) \
	X(MUS_THPPZGSK_MEGAMARI_A) \
	X(MUS_THPPZGSK_MEGAMARI_B) \
	X(MUS_THPPZGSK_MOUNDOFSHIGAN) \
	X(MUS_THPPZGSK_NECROFANTASY) \
	X(MUS_THPPZGSK_SAKEDISH) \
	X(MUS_THPPZGSK_SEPTETTE) \
	X(MUS_THPPZGSK_SHANGHAIALICE) \
	X(MUS_THPPZGSK_SKYSCRAPER) \
	X(MUS_THPPZGSK_THIRDEYE) \
	X(MUS_THPP_DOLLJUDGEMENT) \
	X(MUS_THPP_FAITH) \
	X(MUS_THPP_TEPESSEPTETTE) \
	X(MUS_THPP_NECROFANTASIA) \
	X(MUS_THPP_REINCARNATION) \
	X(MUS_THPP_NOWUNTILTHEMOMENT) \
	X(MUS_THPP_CIVILIZATIONOFMAGIC) \
	X(MUS_THPP_SPRINGLANE) \
	X(MUS_THPP_BELOVEDTOMBOYISHGIRL) \
	X(MUS_THPP_TOMORROWSPECIAL) \
	X(MUS_THPP_THEUMBRELLA) \
	X(MUS_THPP_OLDWORLD) \
	X(MUS_THPP_LULLABYDESERTEDHELL) \
	X(MUS_THPP_ULTIMATETRUTH) \
	X(MUS_THPP_ETERNALMIKO) \
	X(MUS_THPP_LOSTDREAM) \
	X(MUS_THPP_ALICEINWONDERLAND) \
	X(MUS_THPP_SOULASREDASONI) \
	X(MUS_THPP_KIDSFESTIVAL_A) \
	X(MUS_THPP_KIDSFESTIVAL_B) \
	X(MUS_THPP_LUNATEELF) \
	X(MUS_THPP_GENSOKYOGODSLOVED) \
	X(MUS_THPP_HISTORYOFTHEMOON) \
	X(MUS_THPP_CURIOUSSHANGHAI) \
	X(MUS_THPP_DARKROAD) \
	X(MUS_THPP_SKYSCRAPER) \
	X(MUS_THPP_DEAFTOALL_A) \
	X(MUS_THPP_THEBRIDGE) \
	X(MUS_THPP_THIRDEYE) \
	X(MUS_THPP_SEPTETTE) \
	X(MUS_THPP_PLAINASIA) \
	X(MUS_THPP_INTERDIMENSIONALVOYAGE) \
	X(MUS_THPP_ANCIENTTEMPLE) \
	X(MUS_THPP_HEIANALIEN) \
	X(MUS_THPP_CITYOFFLOWER) \
	X(MUS_THPP_ENDOFDAYLIGHT) \
	X(MUS_THPP_BELLOFAVICI) \
	X(MUS_THPP_HAKUREISHRINEGROUND) \
	X(MUS_THPP_BADAPPLE) \
	X(MUS_THPP_GREENWICH) \
	X(MUS_THPP_LEGENDOFHOURAI) \
	X(MUS_THPP_WINDGODGIRL) \
	X(MUS_THPP_CANDIDFRIEND) \
	X(MUS_THPP_THEPUPPET) \
	X(MUS_THPP_DREAMBATTLE) \
	X(MUS_THPP_BROKENMOON) \
	X(MUS_THPP_EDMADLEY) \
	X(MUS_THPP_BOKURANOHISOUTENSOKU_A) \
	X(MUS_THPP_BOKURANOHISOUTENSOKU_B) \
	X(MUS_THPP_DEAFTOALL_B) \
	X(MUS_THPP_PHANTOMENSEMBLE) \
	X(MUS_THPP_IMMORTALSMOKE_A) \
	X(MUS_THPP_IMMORTALSMOKE_B) \
	X(MUS_THPP_INADAHIME) \
	X(MUS_THPP_CORPSEVOYAGE) \
	X(MUS_THPP_JINGLE) \
	X(MUS_THPP_SACREDLOT) \
	X(MUS_THPP_LOSTPLACE) \
	X(MUS_THPP_SAKEDISH) \
	X(MUS_THPP_THEOLDMAN) \
	X(MUS_THPP_DREAMMORESCARLET) \
	X(MUS_THPP_342) \
	X(MUS_THPP_FLYABOVEHATOYAMA_A) \
	X(MUS_THPP_FLYABOVEHATOYAMA_B) \
	X(MUS_THPP_SUPERNEOEXPRESS) \
	X(MUS_THPPZGSK_HISTORYOFTHEMOON) \
	X(MUS_THPPZGSK_DOLLJUDGEMENT) \
	X(MUS_KEY_NAGISA_CUT) \
	X(MUS_INSTRUMENT_TEST_MS_GS_WAVETABLE) \
	X(MUS_THPPAWL_PRIMALSCENE) \
	X(MUS_THPPAWL_REINCARNATION) \
	X(MUS_THPPAWL_CASKETOFSTAR) \
	X(MUS_THPPAWL_PLASTICMIND) \
	X(MUS_THPPAWL_EXTENDASH) \
	X(MUS_THPPAWL_ANCIENTTEMPLE) \
	X(MUS_THPPAWL_KAGOME) \
	X(MUS_THPPRF_MAIDENCAPRICCIO_A) \
	X(MUS_THPPRF_TRUEADMINISTRATOR) \
	X(MUS_THPPRF_367) \
	X(MUS_THPPRF_368) \
	X(MUS_THPPRF_LOVECONSULTATION_A) \
	X(MUS_THPPRF_LITTLEPRINCESS) \
	X(MUS_THPPRF_MARITHEMAGICIAN) \
	X(MUS_THPPRF_372) \
	X(MUS_THPPRF_NECROFANTASIA_A) \
	X(MUS_THPPRF_374) \
	X(MUS_THPPRF_GIRLSSEALINGCLUB) \
	X(MUS_THPPRF_FORESTOFTOHNO) \
	X(MUS_THPPRF_YOUKAISPACETRAVEL) \
	X(MUS_THPPRF_FARSIDEOFTHEMOON) \
	X(MUS_THPPRF_LEGENDOFTOHNO) \
	X(MUS_THPPRF_SOULASRED) \
	X(MUS_THPPRF_SERAPHICCHIKEN) \
	X(MUS_THPPRF_LASTOCCULTISM) \
	X(MUS_THPPRF_MAIDENCAPRICCIO_B) \
	X(MUS_THPPRF_HARTMANNSYOUKAI) \
	X(MUS_THPPRF_NATIVEFAITH) \
	X(MUS_THPPRF_UNKNOWNWASHER) \
	X(MUS_THPPRF_DREAMBATTLE) \
	X(MUS_THPPRF_BADAPPLE) \
	X(MUS_THPPRF_NECROFANTASIA_B) \
	X(MUS_THPPRF_GROUNDSCOLORISYELLOW) \
	X(MUS_THPPRF_WONDERFULHEAVEN) \
	X(MUS_THPPRF_ONIISLAND) \
	X(MUS_THPPRF_BORDEROFLIFE) \
	X(MUS_THPPRF_394) \
	X(MUS_THPPRF_PHANTOMENSEMBLE) \
	X(MUS_THPPRF_NECROFANTASIA_C) \
	X(MUS_THPPRF_STALKTHENIGHT) \
	X(MUS_THPPRF_398) \
	X(MUS_THPPRF_SEPTETTE) \
	X(MUS_THPPRF_REDANDWHITE) \
	X(MUS_THPPRF_PKMN_SM_VS_E4) \
	X(MUS_THPPRF_LOVECONSULTATION_B) \
	X(MUS_THPPRF_EASYGOINGEGOIST) \
	X(MUS_THPPRF_INFINITYBEING) \
	X(MUS_THPPRF_REVERSEIDEOLOGY) \
	X(MUS_THPPRF_406) \
	X(MUS_THPPRF_FLAWLESSCLOTHING) \
	X(MUS_THPPRF_408) \
	X(MUS_THPPRF_SHINKIROU) \
	X(MUS_THPPRF_ANTIMONYTITLE) \
	X(MUS_THPPRF_INNOCENTTREASURES) \
	X(MUS_THPPRF_412) \
	X(MUS_THPPRF_DARKPEGASUS) \
	X(MUS_THPPRF_IDOLATRIZEWORLD) \
	X(MUS_THPPRF_BELOVEDTOMBOYISHGIRL) \
	X(MUS_THPPRF_GENSOKYOGODSLOVED) \
	X(MUS_THPPRF_UNKNOWNX) \
	X(MUS_THPPRF_SKYOFTHESCARLETPERCEPTION) \
	X(MUS_THPPRF_HIDDENSTAR) \
	X(MUS_THPPRF_HEIANALIEN) \
	X(MUS_THPPRF_DEAFTOALL) \
	X(MUS_THPPRF_PUREFURIES) \
	X(MUS_THPPRF_CUTEDEVILINNOCENCE) \
	X(MUS_THPPRF_PKMN_BW2_VS_IRIS) \
	X(MUS_THPPRF_WHITEFLAGUSA) \
	X(MUS_THPPRF_426) \
	X(MUS_THPPRF_MEMENTOOFALLORGANISM) \
	X(MUS_THPPRF_URBANLEGENDS) \
	X(MUS_THPPRF_IMMEMORIALMARKETEER) \
	X(MUS_PC_GBA2_DANZENFWPCMH) \
	X(MUS_PC_GBA2_001) \
	X(MUS_PC_GBA2_002) \
	X(MUS_PC_GBA2_003) \
	X(MUS_PC_GBA2_004) \
	X(MUS_PC_GBA2_005) \
	X(MUS_PC_GBA2_006) \
	X(MUS_PC_GBA2_007) \
	X(MUS_PC_GBA2_008) \
	X(MUS_PC_GBA2_009) \
	X(MUS_PC_GBA2_012) \
	X(MUS_PC_GBA2_013) \
	X(MUS_PC_GBA2_014) \
	X(MUS_PC_GBA2_015) \
	X(MUS_PC_GBA2_016) \
	X(MUS_PC_GBA2_017) \
	X(MUS_PC_GBA2_018) \
	X(MUS_PC_GBA2_019) \
	X(MUS_PC_GBA2_020) \
	X(MUS_PC_GBA2_021) \
	X(MUS_PC_GBA2_023) \
	X(VO_PC_GBA2_DUALAURAWAVE) \
	X(VO_PC_GBA2_CUREBLACK) \
	X(VO_PC_GBA2_CUREWHITE) \
	X(VO_PC_GBA2_SHININGSTREAM) \
	X(VO_PC_GBA2_SHINYLUMINOUS) \
	X(VO_PC_GBA2_EXTREME) \
	X(VO_PC_GBA2_LUMINARIO) \
	X(MUS_PKMN_GSC_CAVES) \
	X(MUS_PKMN_GSC_CHERRYGROVE) \
	X(MUS_PKMN_GSC_CYCLING) \
	X(MUS_PKMN_GSC_ECRUTEAK) \
	X(MUS_PKMN_GSC_ELMLAB) \
	X(MUS_PKMN_GSC_EXITSILVER) \
	X(MUS_PKMN_GSC_GOLDENROD) \
	X(MUS_PKMN_GSC_GYM) \
	X(MUS_PKMN_GSC_ICEPATH) \
	X(MUS_PKMN_GSC_LEAGUE) \
	X(MUS_PKMN_GSC_LIGHTHOUSE) \
	X(MUS_PKMN_GSC_MEETSILVER) \
	X(MUS_PKMN_GSC_NEWBARK) \
	X(MUS_PKMN_GSC_PARK) \
	X(MUS_PKMN_GSC_POKEMONCENTER) \
	X(MUS_PKMN_GSC_ROCKETBASE) \
	X(MUS_PKMN_GSC_ROUTE29) \
	X(MUS_PKMN_GSC_ROUTE30) \
	X(MUS_PKMN_GSC_ROUTE34) \
	X(MUS_PKMN_GSC_ROUTE38) \
	X(MUS_PKMN_GSC_SURFING) \
	X(MUS_PKMN_GSC_TOWER1) \
	X(MUS_PKMN_GSC_TOWER2) \
	X(MUS_PKMN_GSC_VERMILION) \
	X(MUS_PKMN_GSC_VIRIDIAN) \
	X(MUS_PKMN_GSC_VS_GYMLEADER) \
	X(MUS_PKMN_GSC_VS_ROCKET) \
	X(MUS_PKMN_GSC_VS_SILVER) \
	X(MUS_PKMN_GSC_VS_TRAINER) \
	X(MUS_PKMN_GSC_VS_WILD1) \
	X(MUS_PKMN_GSC_VS_WILD2) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM01) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM02) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM03) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM04) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM05) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM06) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM07) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM08) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM09) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM10) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM11) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM12) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM13) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM14) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM15) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM16) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM17) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM18) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM19) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM20) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM21) \
	X(MUS_PC_DS1FWPCMH_SEQ_BGM22) \
	X(MUS_PC_DS2FWPCSS_BGM_ARTS) \
	X(MUS_PC_DS2FWPCSS_BGM_ARTS_02) \
	X(MUS_PC_DS2FWPCSS_BGM_BATTLE_01) \
	X(MUS_PC_DS2FWPCSS_BGM_BATTLE_02) \
	X(MUS_PC_DS2FWPCSS_BGM_CHANGE) \
	X(MUS_PC_DS2FWPCSS_BGM_ED) \
	X(MUS_PC_DS2FWPCSS_BGM_MAP_01) \
	X(MUS_PC_DS2FWPCSS_BGM_MAP_02) \
	X(MUS_PC_DS2FWPCSS_BGM_MAP_03) \
	X(MUS_PC_DS2FWPCSS_BGM_MAP_04) \
	X(MUS_PC_DS2FWPCSS_BGM_MENU) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_01) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_02) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_03) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_04) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_05) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_06) \
	X(MUS_PC_DS2FWPCSS_BGM_MINIGAME_07) \
	X(MUS_PC_DS2FWPCSS_BGM_RESULT) \
	X(MUS_PC_DS2FWPCSS_BGM_STORY_01) \
	X(MUS_PC_DS2FWPCSS_BGM_STORY_02) \
	X(MUS_PC_DS2FWPCSS_BGM_STORY_03) \
	X(MUS_PC_DS2FWPCSS_BGM_STORY_04) \
	X(MUS_PC_DS2FWPCSS_BGM_STORY_05) \
	X(MUS_PC_DS2FWPCSS_BGM_TITLE) \
	X(MUS_PC_DS2FWPCSS_JGL_SUBTITLE) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE01) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE02) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE03) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE04) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE06) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE07) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE08) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE09) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE11) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE12) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE13) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE14) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE15) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE16) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE17) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE19) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE20) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE21) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE22) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE23) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE25) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE26) \
	X(MUS_PC_DS3PC5_SEQ_PRICURE27) \
	X(MUS_PC_DS4PC5GOGO_BGM_ADV_01) \
	X(MUS_PC_DS4PC5GOGO_BGM_ADV_02) \
	X(MUS_PC_DS4PC5GOGO_BGM_ADV_03) \
	X(MUS_PC_DS4PC5GOGO_BGM_ADV_04) \
	X(MUS_PC_DS4PC5GOGO_BGM_BTL_01) \
	X(MUS_PC_DS4PC5GOGO_BGM_BTL_02) \
	X(MUS_PC_DS4PC5GOGO_BGM_BTL_03) \
	X(MUS_PC_DS4PC5GOGO_BGM_BTL_04) \
	X(MUS_PC_DS4PC5GOGO_BGM_BTL_05) \
	X(MUS_PC_DS4PC5GOGO_BGM_MENU) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_01) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_02) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_03) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_04) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_05) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_06) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_07) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_08) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_09) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_10) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_11) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_12) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_13) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_14) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_15) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_16) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_17) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_18) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_19) \
	X(MUS_PC_DS4PC5GOGO_BGM_MG_20) \
	X(MUS_PC_DS4PC5GOGO_BGM_TITLE) \
	X(MUS_PC_DS5FPC_BGM_ARTS) \
	X(MUS_PC_DS5FPC_BGM_BATTLE) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_01A) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_01B) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_02) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_03) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_04) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_05) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_06A) \
	X(MUS_PC_DS5FPC_BGM_BATTLE_06B) \
	X(MUS_PC_DS5FPC_BGM_CHANGE) \
	X(MUS_PC_DS5FPC_BGM_MAP) \
	X(MUS_PC_DS5FPC_BGM_MG_01) \
	X(MUS_PC_DS5FPC_BGM_MG_02) \
	X(MUS_PC_DS5FPC_BGM_MG_03) \
	X(MUS_PC_DS5FPC_BGM_MG_04) \
	X(MUS_PC_DS5FPC_BGM_MG_05) \
	X(MUS_PC_DS5FPC_BGM_MG_06) \
	X(MUS_PC_DS5FPC_BGM_MG_07) \
	X(MUS_PC_DS5FPC_BGM_MG_08) \
	X(MUS_PC_DS5FPC_BGM_MG_09) \
	X(MUS_PC_DS5FPC_BGM_MG_10) \
	X(MUS_PC_DS5FPC_BGM_MG_11) \
	X(MUS_PC_DS5FPC_BGM_RESULT_01) \
	X(MUS_PC_DS5FPC_BGM_TITLE) \
	X(MUS_PC_DS5FPC_BGM_TUTORIAL) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_00) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_01) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_02) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_03) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_04) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_05) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_06) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_07) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_08) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_09) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_10) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_11) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_12) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_13) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_14) \
	X(MUS_PC_DS6HCPC_SEQ_BGM_BGM_15) \
	X(MUS_PC_DS6HCPC_SEQ_JGL_JIN_00) \
	X(MUS_PC_DS6HCPC_SEQ_JGL_JIN_01) \
	X(MUS_PC_DS7HCPC_BGM00) \
	X(MUS_PC_DS7HCPC_BGM01) \
	X(MUS_PC_DS7HCPC_BGM02) \
	X(MUS_PC_DS7HCPC_BGM03) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_00) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_01) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_02) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_03) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_04) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_05) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_06) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_08) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_09) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_10) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_11) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_12) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_13) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_15) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_17) \
	X(MUS_PC_DS8SPC_SEQ_BGM_BGM_19) \
	X(MUS_DCD_JTD_SYMPHONY_LOOP) \
	X(MUS_DGMN_DW1_CURLING) \
	X(MUS_DGMN_DW1_FACTORIALTOWN_DAY) \
	X(MUS_DGMN_DW1_FACTORIALTOWN_NIGHT) \
	X(MUS_DGMN_DW1_FILECITY_DAY) \
	X(MUS_DGMN_DW1_FILECITY_NIGHT) \
	X(MUS_DGMN_DW1_GEARSAVANNA) \
	X(MUS_DGMN_DW1_MONOCHROMON) \
	X(MUS_DGMN_DW1_VS_DIGIMONEVENT) \
	X(MUS_DGMN_DW1_VS_MACHINEDRAMON) \
	X(MUS_DGMN_DW1_WARUMONZAEMON) \
	X(MUS_PKMN_PMDRED_001) \
	X(MUS_PKMN_PMDRED_002) \
	X(MUS_PKMN_PMDRED_003) \
	X(MUS_PKMN_PMDRED_004) \
	X(MUS_PKMN_PMDRED_005) \
	X(MUS_PKMN_PMDRED_006) \
	X(MUS_PKMN_PMDRED_007) \
	X(MUS_PKMN_PMDRED_008) \
	X(MUS_PKMN_PMDRED_009) \
	X(MUS_PKMN_PMDRED_010) \
	X(MUS_PKMN_PMDRED_011) \
	X(MUS_PKMN_PMDRED_012) \
	X(MUS_PKMN_PMDRED_013) \
	X(MUS_PKMN_PMDRED_014) \
	X(MUS_PKMN_PMDRED_015) \
	X(MUS_PKMN_PMDRED_016) \
	X(MUS_PKMN_PMDRED_017) \
	X(MUS_PKMN_PMDRED_018) \
	X(MUS_PKMN_PMDRED_019) \
	X(MUS_PKMN_PMDRED_020) \
	X(MUS_PKMN_PMDRED_021) \
	X(MUS_PKMN_PMDRED_022) \
	X(MUS_PKMN_PMDRED_023) \
	X(MUS_PKMN_PMDRED_024) \
	X(MUS_PKMN_PMDRED_025) \
	X(MUS_PKMN_PMDRED_026) \
	X(MUS_PKMN_PMDRED_027) \
	X(MUS_PKMN_PMDRED_028) \
	X(MUS_PKMN_PMDRED_029) \
	X(MUS_PKMN_PMDRED_030) \
	X(MUS_PKMN_PMDRED_031) \
	X(MUS_PKMN_PMDRED_032) \
	X(MUS_PKMN_PMDRED_033) \
	X(MUS_PKMN_PMDRED_034) \
	X(MUS_PKMN_PMDRED_035) \
	X(MUS_PKMN_PMDRED_036) \
	X(MUS_PKMN_PMDRED_037) \
	X(MUS_PKMN_PMDRED_038) \
	X(MUS_PKMN_PMDRED_039) \
	X(MUS_PKMN_PMDRED_040) \
	X(MUS_PKMN_PMDRED_041) \
	X(MUS_PKMN_PMDRED_042) \
	X(MUS_PKMN_PMDRED_043) \
	X(MUS_PKMN_PMDRED_044) \
	X(MUS_PKMN_PMDRED_045) \
	X(MUS_PKMN_PMDRED_046) \
	X(MUS_PKMN_PMDRED_050) \
	X(MUS_PKMN_PMDRED_051) \
	X(MUS_PKMN_PMDRED_052) \
	X(MUS_PKMN_PMDRED_100) \
	X(MUS_PKMN_PMDRED_101) \
	X(MUS_PKMN_PMDRED_102) \
	X(MUS_PKMN_PMDRED_103) \
	X(MUS_PKMN_PMDRED_104) \
	X(MUS_PKMN_PMDRED_105) \
	X(MUS_PKMN_PMDRED_106) \
	X(MUS_PKMN_PMDRED_107) \
	X(MUS_PKMN_PMDRED_108) \
	X(MUS_PKMN_PMDRED_109) \
	X(MUS_PKMN_PMDRED_110) \
	X(MUS_PKMN_PMDRED_111) \
	X(MUS_PKMN_PMDRED_112) \
	X(MUS_PKMN_PMDRED_113) \
	X(MUS_PKMN_PMDRED_114) \
	X(MUS_PKMN_PMDRED_115) \
	X(MUS_PKMN_PMDRED_116) \
	X(MUS_PKMN_PMDRED_117) \
	X(MUS_PKMN_PMDRED_118) \
	X(MUS_PKMN_PMDRED_119) \
	X(MUS_PKMN_PMDRED_120) \
	X(MUS_PKMN_PMDRED_121) \
	X(MUS_PKMN_PMDRED_122) \
	X(MUS_PKMN_PMDRED_123) \
	X(MUS_PKMN_PMDRED_124) \
	X(MUS_PKMN_PMDRED_125) \
	X(MUS_PKMN_PMDRED_126) \
	X(MUS_PKMN_PMDRED_127) \
	X(MUS_PKMN_PMDRED_128) \
	X(MUS_DGMN_STORY1_DIGIFARM) \
	X(MUS_DGMN_STORY1_INTERIOR) \
	X(MUS_DGMN_STORY1_VS_BOSS) \
	X(MUS_DGMN_STORY1_VS_DIGIMON) \
	X(MUS_DGMN_STORY1_VS_DIGIMON_DMW) \
	X(MUS_DGMN_STORY2_BOARD_ANGEL) \
	X(MUS_DGMN_STORY2_BOARD_BEAST) \
	X(MUS_DGMN_STORY2_BOARD_BIRD) \
	X(MUS_DGMN_STORY2_BOARD_DEVIL) \
	X(MUS_DGMN_STORY2_BOARD_DRAGON) \
	X(MUS_DGMN_STORY2_BOARD_FOREST) \
	X(MUS_DGMN_STORY2_BOARD_MECH) \
	X(MUS_DGMN_STORY2_BOARD_SEA) \
	X(MUS_DGMN_STORY2_DARKMOON_CITY) \
	X(MUS_DGMN_STORY2_DIGIFARM) \
	X(MUS_DGMN_STORY2_SUNSHINE_CITY) \
	X(MUS_DGMN_STORY2_VS_ADVANCE) \
	X(MUS_DGMN_STORY2_VS_BOSS) \
	X(MUS_DGMN_STORY2_VS_CHAOS_BRAIN) \
	X(MUS_DGMN_STORY2_VS_DIGIMON) \
	X(MUS_DGMN_STORY2_VS_FINAL_BOSS) \
	X(MUS_DGMN_STORY2_VS_MULTIPLAYER) \
	X(MUS_PC_ALLHENSHIN) \

#define SOUND_LIST_SE \
    X(SE_USE_ITEM) \
    X(SE_PC_LOGIN) \
    X(SE_PC_OFF) \
    X(SE_PC_ON) \
    X(SE_SELECT) \
    X(SE_WIN_OPEN) \
    X(SE_WALL_HIT) \
    X(SE_DOOR) \
    X(SE_EXIT) \
    X(SE_LEDGE) \
    X(SE_BIKE_BELL) \
    X(SE_NOT_EFFECTIVE) \
    X(SE_EFFECTIVE) \
    X(SE_SUPER_EFFECTIVE) \
    X(SE_BALL_OPEN) \
    X(SE_FAINT) \
    X(SE_FLEE) \
    X(SE_SLIDING_DOOR) \
    X(SE_SHIP) \
    X(SE_BANG) \
    X(SE_PIN) \
    X(SE_BOO) \
    X(SE_BALL) \
    X(SE_CONTEST_PLACE) \
    X(SE_A) \
    X(SE_I) \
    X(SE_U) \
    X(SE_E) \
    X(SE_O) \
    X(SE_N) \
    X(SE_SUCCESS) \
    X(SE_FAILURE) \
    X(SE_EXP) \
    X(SE_BIKE_HOP) \
    X(SE_SWITCH) \
    X(SE_CLICK) \
    X(SE_FU_ZAKU) \
    X(SE_CONTEST_CONDITION_LOSE) \
    X(SE_LAVARIDGE_FALL_WARP) \
    X(SE_ICE_STAIRS) \
    X(SE_ICE_BREAK) \
    X(SE_ICE_CRACK) \
    X(SE_FALL) \
    X(SE_UNLOCK) \
    X(SE_WARP_IN) \
    X(SE_WARP_OUT) \
    X(SE_REPEL) \
    X(SE_ROTATING_GATE) \
    X(SE_TRUCK_MOVE) \
    X(SE_TRUCK_STOP) \
    X(SE_TRUCK_UNLOAD) \
    X(SE_TRUCK_DOOR) \
    X(SE_BERRY_BLENDER) \
    X(SE_CARD) \
    X(SE_SAVE) \
    X(SE_BALL_BOUNCE_1) \
    X(SE_BALL_BOUNCE_2) \
    X(SE_BALL_BOUNCE_3) \
    X(SE_BALL_BOUNCE_4) \
    X(SE_BALL_TRADE) \
    X(SE_BALL_THROW) \
    X(SE_NOTE_C) \
    X(SE_NOTE_D) \
    X(SE_NOTE_E) \
    X(SE_NOTE_F) \
    X(SE_NOTE_G) \
    X(SE_NOTE_A) \
    X(SE_NOTE_B) \
    X(SE_NOTE_C_HIGH) \
    X(SE_PUDDLE) \
    X(SE_BRIDGE_WALK) \
    X(SE_ITEMFINDER) \
    X(SE_DING_DONG) \
    X(SE_BALLOON_RED) \
    X(SE_BALLOON_BLUE) \
    X(SE_BALLOON_YELLOW) \
    X(SE_BREAKABLE_DOOR) \
    X(SE_MUD_BALL) \
    X(SE_FIELD_POISON) \
    X(SE_ESCALATOR) \
    X(SE_THUNDERSTORM) \
    X(SE_THUNDERSTORM_STOP) \
    X(SE_DOWNPOUR) \
    X(SE_DOWNPOUR_STOP) \
    X(SE_RAIN) \
    X(SE_RAIN_STOP) \
    X(SE_THUNDER) \
    X(SE_THUNDER2) \
    X(SE_ELEVATOR) \
    X(SE_LOW_HEALTH) \
    X(SE_EXP_MAX) \
    X(SE_ROULETTE_BALL) \
    X(SE_ROULETTE_BALL2) \
    X(SE_TAILLOW_WING_FLAP) \
    X(SE_SHOP) \
    X(SE_CONTEST_HEART) \
    X(SE_CONTEST_CURTAIN_RISE) \
    X(SE_CONTEST_CURTAIN_FALL) \
    X(SE_CONTEST_ICON_CHANGE) \
    X(SE_CONTEST_ICON_CLEAR) \
    X(SE_CONTEST_MONS_TURN) \
    X(SE_SHINY) \
    X(SE_INTRO_BLAST) \
    X(SE_MUGSHOT) \
    X(SE_APPLAUSE) \
    X(SE_VEND) \
    X(SE_ORB) \
    X(SE_DEX_SCROLL) \
    X(SE_DEX_PAGE) \
    X(SE_POKENAV_ON) \
    X(SE_POKENAV_OFF) \
    X(SE_DEX_SEARCH) \
    X(SE_EGG_HATCH) \
    X(SE_BALL_TRAY_ENTER) \
    X(SE_BALL_TRAY_BALL) \
    X(SE_BALL_TRAY_EXIT) \
    X(SE_GLASS_FLUTE) \
    X(SE_M_THUNDERBOLT) \
    X(SE_M_THUNDERBOLT2) \
    X(SE_M_HARDEN) \
    X(SE_M_NIGHTMARE) \
    X(SE_M_VITAL_THROW) \
    X(SE_M_VITAL_THROW2) \
    X(SE_M_BUBBLE) \
    X(SE_M_BUBBLE2) \
    X(SE_M_BUBBLE3) \
    X(SE_M_RAIN_DANCE) \
    X(SE_M_CUT) \
    X(SE_M_STRING_SHOT) \
    X(SE_M_STRING_SHOT2) \
    X(SE_M_ROCK_THROW) \
    X(SE_M_GUST) \
    X(SE_M_GUST2) \
    X(SE_M_DOUBLE_SLAP) \
    X(SE_M_DOUBLE_TEAM) \
    X(SE_M_RAZOR_WIND) \
    X(SE_M_ICY_WIND) \
    X(SE_M_THUNDER_WAVE) \
    X(SE_M_COMET_PUNCH) \
    X(SE_M_MEGA_KICK) \
    X(SE_M_MEGA_KICK2) \
    X(SE_M_CRABHAMMER) \
    X(SE_M_JUMP_KICK) \
    X(SE_M_FLAME_WHEEL) \
    X(SE_M_FLAME_WHEEL2) \
    X(SE_M_FLAMETHROWER) \
    X(SE_M_FIRE_PUNCH) \
    X(SE_M_TOXIC) \
    X(SE_M_SACRED_FIRE) \
    X(SE_M_SACRED_FIRE2) \
    X(SE_M_EMBER) \
    X(SE_M_TAKE_DOWN) \
    X(SE_M_BLIZZARD) \
    X(SE_M_BLIZZARD2) \
    X(SE_M_SCRATCH) \
    X(SE_M_VICEGRIP) \
    X(SE_M_WING_ATTACK) \
    X(SE_M_FLY) \
    X(SE_M_SAND_ATTACK) \
    X(SE_M_RAZOR_WIND2) \
    X(SE_M_BITE) \
    X(SE_M_HEADBUTT) \
    X(SE_M_SURF) \
    X(SE_M_HYDRO_PUMP) \
    X(SE_M_WHIRLPOOL) \
    X(SE_M_HORN_ATTACK) \
    X(SE_M_TAIL_WHIP) \
    X(SE_M_MIST) \
    X(SE_M_POISON_POWDER) \
    X(SE_M_BIND) \
    X(SE_M_DRAGON_RAGE) \
    X(SE_M_SING) \
    X(SE_M_PERISH_SONG) \
    X(SE_M_PAY_DAY) \
    X(SE_M_DIG) \
    X(SE_M_DIZZY_PUNCH) \
    X(SE_M_SELF_DESTRUCT) \
    X(SE_M_EXPLOSION) \
    X(SE_M_ABSORB_2) \
    X(SE_M_ABSORB) \
    X(SE_M_SCREECH) \
    X(SE_M_BUBBLE_BEAM) \
    X(SE_M_BUBBLE_BEAM2) \
    X(SE_M_SUPERSONIC) \
    X(SE_M_BELLY_DRUM) \
    X(SE_M_METRONOME) \
    X(SE_M_BONEMERANG) \
    X(SE_M_LICK) \
    X(SE_M_PSYBEAM) \
    X(SE_M_FAINT_ATTACK) \
    X(SE_M_SWORDS_DANCE) \
    X(SE_M_LEER) \
    X(SE_M_SWAGGER) \
    X(SE_M_SWAGGER2) \
    X(SE_M_HEAL_BELL) \
    X(SE_M_CONFUSE_RAY) \
    X(SE_M_SNORE) \
    X(SE_M_BRICK_BREAK) \
    X(SE_M_GIGA_DRAIN) \
    X(SE_M_PSYBEAM2) \
    X(SE_M_SOLAR_BEAM) \
    X(SE_M_PETAL_DANCE) \
    X(SE_M_TELEPORT) \
    X(SE_M_MINIMIZE) \
    X(SE_M_SKETCH) \
    X(SE_M_SWIFT) \
    X(SE_M_REFLECT) \
    X(SE_M_BARRIER) \
    X(SE_M_DETECT) \
    X(SE_M_LOCK_ON) \
    X(SE_M_MOONLIGHT) \
    X(SE_M_CHARM) \
    X(SE_M_CHARGE) \
    X(SE_M_STRENGTH) \
    X(SE_M_HYPER_BEAM) \
    X(SE_M_WATERFALL) \
    X(SE_M_REVERSAL) \
    X(SE_M_ACID_ARMOR) \
    X(SE_M_SANDSTORM) \
    X(SE_M_TRI_ATTACK) \
    X(SE_M_TRI_ATTACK2) \
    X(SE_M_ENCORE) \
    X(SE_M_ENCORE2) \
    X(SE_M_BATON_PASS) \
    X(SE_M_MILK_DRINK) \
    X(SE_M_ATTRACT) \
    X(SE_M_ATTRACT2) \
    X(SE_M_MORNING_SUN) \
    X(SE_M_FLATTER) \
    X(SE_M_SAND_TOMB) \
    X(SE_M_GRASSWHISTLE) \
    X(SE_M_SPIT_UP) \
    X(SE_M_DIVE) \
    X(SE_M_EARTHQUAKE) \
    X(SE_M_TWISTER) \
    X(SE_M_SWEET_SCENT) \
    X(SE_M_YAWN) \
    X(SE_M_SKY_UPPERCUT) \
    X(SE_M_STAT_INCREASE) \
    X(SE_M_HEAT_WAVE) \
    X(SE_M_UPROAR) \
    X(SE_M_HAIL) \
    X(SE_M_COSMIC_POWER) \
    X(SE_M_TEETER_DANCE) \
    X(SE_M_STAT_DECREASE) \
    X(SE_M_HAZE) \
    X(SE_M_HYPER_BEAM2) \
    X(SE_RG_DOOR) \
    X(SE_RG_CARD_FLIP) \
    X(SE_RG_CARD_FLIPPING) \
    X(SE_RG_CARD_OPEN) \
    X(SE_RG_BAG_CURSOR) \
    X(SE_RG_BAG_POCKET) \
    X(SE_RG_BALL_CLICK) \
    X(SE_RG_SHOP) \
    X(SE_RG_SS_ANNE_HORN) \
    X(SE_RG_HELP_OPEN) \
    X(SE_RG_HELP_CLOSE) \
    X(SE_RG_HELP_ERROR) \
    X(SE_RG_DEOXYS_MOVE) \
    X(SE_RG_POKE_JUMP_SUCCESS) \
    X(SE_RG_POKE_JUMP_FAILURE) \
    X(SE_PHONE_CALL) \
    X(SE_PHONE_CLICK) \
    X(SE_ARENA_TIMEUP1) \
    X(SE_ARENA_TIMEUP2) \
    X(SE_PIKE_CURTAIN_CLOSE) \
    X(SE_PIKE_CURTAIN_OPEN) \
    X(SE_SUDOWOODO_SHAKE) \

// Create BGM list
#define X(songId) static const u8 sBGMName_##songId[] = _(#songId);
SOUND_LIST_BGM
#undef X

#define X(songId) sBGMName_##songId,
static const u8 *const sBGMNames[] =
{
SOUND_LIST_BGM
};
#undef X

// Create SE list
#define X(songId) static const u8 sSEName_##songId[] = _(#songId);
SOUND_LIST_SE
#undef X

#define X(songId) sSEName_##songId,
static const u8 *const sSENames[] =
{
SOUND_LIST_SE
};
#undef X

#endif //DEBUG_OVERWORLD_MENU == TRUE
