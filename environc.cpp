with Ada; use Ada;
with Ada.Strings; use Ada.Strings; -- overrides our "strings" package
with Ada.Strings.Bounded;
with Ada.Strings.Maps;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Characters; use Ada.Characters;
with Ada.Characters.Conversions;  use Ada.Characters.Conversions;
with Ada.Characters.Handling; use Ada.Characters.Handling;

Package environa is

package Strings255 is new Ada.Strings.Bounded.Generic_Bounded_Length ( Max => 255 );
use Strings255;
subtype Str255 is Strings255.Bounded_String;
NullStr255 : str255 renames Strings255.Null_Bounded_String;

subTYPE String255Fixedtype is String(1..255);

function To255( Source : in String; Drop : in Strings.Truncation := Strings.Error ) return Str255 renames Strings255.To_Bounded_String; pragma Pure_Function( To255 );
function ToString( source : in str255 ) return String renames Strings255.To_String; pragma Pure_Function( ToString );

-- Renaming these functions makes them automatically present with common from TextTools.  I am now questioning whether these are needed here.
-- So I commented them out and will see if it still works.  

-- function Length (Source : in Str255) return Strings255.Length_Range renames Strings255.Length;
-- function Append (Left, Right : in Str255; Drop   : in Strings.Truncation  := Strings.Error) return Str255 renames Strings255.Append;
-- function Append (Left : in Str255; Right : in String; Drop  : in Strings.Truncation := Strings.Error) return  Str255 renames Strings255.Append;
-- function Append (Left : in String; Right : in Str255; Drop  : in Strings.Truncation := Strings.Error) return  Str255 renames Strings255.Append;
-- function Append (Left  : in Str255; Right : in Character; Drop  : in Strings.Truncation := Strings.Error) return  Str255 renames Strings255.Append;
-- function Append (Left  : in Character; Right : in Str255; Drop  : in Strings.Truncation := Strings.Error) return  Str255 renames Strings255.Append;
-- procedure Append (Source   : in out Str255; New_Item : in Str255; Drop     : in Strings.Truncation  := Strings.Error) renames Strings255.Append;
-- procedure Append (Source   : in out Str255; New_Item : in String; Drop     : in Strings.Truncation  := Strings.Error) renames Strings255.Append;
-- procedure Append (Source   : in out Str255; New_Item : in Character; Drop     : in Strings.Truncation  := Strings.Error) renames Strings255.Append;
-- 
-- function "&" (Left, Right : in Str255) return Str255 renames Strings255."&";
-- function "&" (Left : in Str255; Right : in String) return Str255 renames Strings255."&";
-- function "&" (Left : in String; Right : in Str255) return Str255 renames Strings255."&";
-- function "&" (Left : in Str255; Right : in Character) return Str255 renames Strings255."&";
-- function "&" (Left : in Character; Right : in Str255) return Str255 renames Strings255."&";
-- function Element (Source : in Str255; Index : in Positive) return Character renames Strings255.Element;
-- procedure Replace_Element (Source : in out Str255; Index : in Positive; By : in Character) renames Strings255.Replace_Element;
-- function Slice (Source : in Str255; Low : in Positive; High : in Natural) return String renames Strings255.Slice;
-- function "=" (Left, Right : in Str255) return Boolean renames Strings255."=";
-- function "=" (Left : in Str255; Right : in String) return  Boolean renames Strings255."=";
-- function "=" (Left : in String; Right : in Str255) return Boolean renames Strings255."=";
-- function "<" (Left, Right : in Str255) return Boolean renames Strings255."<";
-- function "<" (Left : in Str255; Right : in String) return Boolean renames Strings255."<";
-- function "<" (Left : in String; Right : in Str255) return Boolean renames Strings255."<";
-- function "<=" (Left, Right : in Str255) return Boolean renames Strings255."<=";
-- function "<=" (Left : in Str255; Right : in String) return Boolean renames Strings255."<=";
-- function "<=" (Left : in String; Right : in Str255) return Boolean renames Strings255."<=";
-- function ">"  (Left, Right : in Str255) return Boolean renames Strings255.">";
-- function ">" (Left : in Str255; Right : in String) return Boolean renames Strings255.">";
-- function ">" (Left : in String; Right : in Str255) return Boolean renames Strings255.">";
-- function ">=" (Left, Right : in Str255) return Boolean renames Strings255.">=";
-- function ">=" (Left : in Str255; Right : in String) return Boolean renames Strings255.">=";
-- function ">=" (Left : in String; Right : in Str255) return Boolean renames Strings255.">=";
-- function Index (Source : in Str255; Pattern : in String; Going : in Strings.Direction := Strings.Forward; Mapping : in Maps.Character_Mapping := Maps.Identity) return Natural renames Strings255.Index;
-- function Index (Source : in Str255; Pattern : in String; Going : in Strings.Direction := Strings.Forward; Mapping : in Maps.Character_Mapping_Function) return Natural renames Strings255.Index;
-- function Index (Source : in Str255; Set : in Maps.Character_Set; Test : in Strings.Membership := Strings.Inside; Going : in Strings.Direction  := Strings.Forward) return Natural renames Strings255.Index;
-- function Index_Non_Blank (Source : in Str255; Going : in Strings.Direction := Strings.Forward) return Natural renames Strings255.Index_Non_Blank;
-- function Count (Source : in Str255; Pattern : in String; Mapping : in Maps.Character_Mapping := Maps.Identity) return Natural renames Strings255.Count;
-- function Count (Source : in Str255; Pattern : in String; Mapping : in Maps.Character_Mapping_Function) return Natural renames Strings255.Count;
--  function Count (Source : in Str255; Set : in Maps.Character_Set) return Natural renames Strings255.Count;
--  procedure Find_Token (Source : in Str255; Set : in Maps.Character_Set; Test : in Strings.Membership; First : out Positive; Last : out Natural) renames Strings255.Find_Token;
--  function Translate (Source : in Str255; Mapping : in Maps.Character_Mapping) return Str255 renames Strings255.Translate;
--  procedure Translate (Source : in out Str255; Mapping : in Maps.Character_Mapping) renames Strings255.Translate;
--  function Translate (Source : in Str255; Mapping : in Maps.Character_Mapping_Function) return  Str255 renames Strings255.Translate;
--  procedure Translate (Source : in out Str255; Mapping : in Maps.Character_Mapping_Function) renames Strings255.Translate;
--  function Replace_Slice (Source : in Str255; Low : in Positive; High : in Natural; By : in String; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Replace_Slice;
--  procedure Replace_Slice (Source : in out Str255; Low : in Positive; High : in Natural; By : in String; Drop : in Strings.Truncation := Strings.Error) renames Strings255.Replace_Slice;
--  function Insert (Source: in Str255; Before : in Positive; New_Item : in String; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Insert; procedure Insert (Source : in out Str255; Before : in Positive; New_Item : in String; Drop : in Strings.Truncation := Strings.Error) renames Strings255.Insert;
-- function Overwrite (Source : in Str255; Position : in Positive; New_Item : in String; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Overwrite;
-- procedure Overwrite (Source : in out Str255; Position : in Positive; New_Item : in String; Drop : in Strings.Truncation := Strings.Error) renames Strings255.Overwrite;
-- function Delete (Source : in Str255; From : in Positive; Through : in Natural) return Str255 renames Strings255.Delete;
-- procedure Delete (Source : in out Str255; From : in Positive; Through : in Natural) renames Strings255.Delete;
-- function Trim (Source : in Str255; Side : in Strings.Trim_End) return Str255 renames Strings255.Trim;
-- procedure Trim (Source : in out Str255; Side : in Strings.Trim_End) renames Strings255.Trim;
-- function Trim (Source : in Str255; Left : in Maps.Character_Set; Right : in Maps.Character_Set) return Str255 renames Strings255.Trim;
-- procedure Trim (Source : in out Str255; Left : in Maps.Character_Set; Right : in Maps.Character_Set) renames Strings255.Trim;
-- function Head (Source : in Str255; Count : in Natural; Pad : in Character := Strings.Space; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Head;
-- procedure Head (Source : in out Str255; Count : in Natural; Pad : in Character := Strings.Space; Drop : in Strings.Truncation := Strings.Error) renames Strings255.Head;
-- function Tail (Source : in Str255; Count : in Natural; Pad : in Character := ' '; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Tail;
-- procedure Tail (Source : in out Str255; Count : in Natural; Pad : in Character := ' '; Drop : in Strings.Truncation := Strings.Error) renames Strings255.Tail;
-- function "*" (Left : in Natural; Right : in Character) return Str255 renames Strings255."*";
-- function "*" (Left : in Natural; Right : in String) return Str255 renames Strings255."*";
-- function "*" (Left : in Natural; Right : in Str255) return  Str255 renames Strings255."*";
-- function Replicate (Count : in Natural; Item : in Character; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Replicate;
-- function Replicate (Count : in Natural; Item : in String; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Replicate;
-- function Replicate (Count : in Natural; Item : in Str255; Drop : in Strings.Truncation := Strings.Error) return Str255 renames Strings255.Replicate;
-- 
function ToInteger( s : in str255 ) return integer;
function ToInteger( s : in string ) return integer;
function ToLongInteger( s : in str255 ) return long_integer;
function ToLongInteger( s : in string ) return long_integer;

function CAP(Item : Character) return Character renames Ada.Characters.Handling.To_Upper;
function CAP(Item : String) return String renames Ada.Characters.Handling.To_Upper;

Function Get_Command_Line return Unbounded_String; -- iterative using unbounded strings for all parts now.
Function Get_Command_Line return String; -- iterative approach using unbounded strings for the intermediate string parts
Function Get_A_Command_Line return String;  -- iterative approach using declare block, fixed string and slices
Procedure Get_A_Command_Line(cmdline : out String; Last : out Natural); 
Function GetCommandLine return String;  -- recursive approach
PROCEDURE GetCommandLine(CommandLine : OUT Str255);

END environa;
with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Command_Line; use Ada.Command_Line;
--        function Argument_Count return Natural;
--        function Argument (Number : in Positive) return String;
--        function Command_Name return String;
--        type Exit_Status is implementation-defined integer type;
--        Success : constant Exit_Status;
--        Failure : constant Exit_Status;
--        procedure Set_Exit_Status (Code : in Exit_Status);
with Ada.Characters; use Ada.Characters;
with Ada.Characters.Conversions;  use Ada.Characters.Conversions;
with Ada.Characters.Handling; use Ada.Characters.Handling;
with Ada.Characters.Latin_1; use Ada.Characters.Latin_1;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

Package body environa is

--        FROM Args IMPORT GetArg, Narg;
--        FROM Strings IMPORT String1, Length, Assign, Delete, Insert, Replace, Append, Concat, Capitalize;
--        FROM UTILLIBg IMPORT STRTYP;
--        FROM MiscStdInOutg IMPORT WriteString, WriteLn, WriteCard, PressAnyKey;
--        IMPORT ASCII;

--  subtype Cardinal is mod 2**32;

  c,k,n                   : Natural;
  i,j                     : INTEGER;
  cmdlinefrag, argstr     : Str255;
  ok                      : BOOLEAN;

function ToInteger(s : in str255) return integer is
  pos  : integer := 1;
  sign : integer := 1;
  i    : integer := 0;
begin
  if length( s ) > 0 then
    while Element( s, pos ) = ' ' and then pos < Length( s ) loop
      pos := pos + 1;
    end loop;
    if Element( s, pos ) = '-' then
      sign := -1;
      pos := pos + 1;
    elsif Element( s, pos ) = '+' then
      pos := pos + 1;
    end if;
    i := integer'value( ToString( Tail( s, length(s)-pos+1 ) ) );
  end if;
  return i * sign;
end ToInteger;

function ToInteger(s : in string) return integer is

begin
  return ToInteger(To255(s));
end ToInteger;

function ToLongInteger(s : in Str255) return long_integer is
  pos  : integer := 1;
  sign : long_integer := 1;
  l    : long_integer := 0;
begin
  if length( s ) > 0 then
     while Element( s, pos ) = ' ' and then pos < Length( s ) loop
       pos := pos + 1;
     end loop;
     if Element( s, pos ) = '-' then
        sign := -1;
        pos := pos + 1;
     elsif Element( s, pos ) = '+' then
        pos := pos + 1;
     end if;
     l := long_integer'value( ToString( Tail( s, length(s)-pos+1 ) ) );
  end if;
  return l * sign;
end ToLongInteger;

function ToLongInteger(s : in string) return long_integer is
begin
  return ToLongInteger( To255( s ) );
end ToLongInteger;

Function GetCommandLine return String is
-- Uses recursive solution to the fixed string problem.  Credit to Jeffrey Carter.
  Function Piece (arg: positive) return string is
  Begin --piece
    IF arg > Argument_Count THEN
      Return "";
    ELSIF arg = Argument_Count THEN
      Return Argument(Arg);
    ELSE
      Return Argument(Arg) & ' ' & Piece(Arg+1);
    END IF;
  End Piece;

Begin -- GetCommandLine
  IF Argument_Count > 0 THEN
    Return Piece(1);
  ELSE
    Return "";
  END IF;
END GetCommandLine;

Function Get_A_Command_Line return String is
-- This is an iterative way to return a fixed string type.  It first loops to count how much length is needed,
-- and then loops again to assemble the pieces, all using a Declare block, only fixed strings, and slices.
-- Credit to Brian Drummond.
  len : Natural := 0;

BEGIN  
  IF Argument_Count = 0 THEN
    Return "";
  END IF;
  FOR I in 1 .. Argument_Count LOOP
    len := len + argument(I)'Length;
  END LOOP;
  len := len + Argument_Count;  -- to also count the spaces including the one at the end.  I'm not sure what will happen to that one later.
  DECLARE
    result : String(1..len);
    posn : Natural := 1;
  Begin
    FOR I in 1 .. Argument_Count LOOP
      result(posn .. posn+Argument(I)'Length) := Argument(I) & ' ';
      posn := posn + Argument(I)'Length + 1;  --the +1 factor is to account for the space.
    END LOOP;
    Return result;
  END; -- declare block
END Get_A_Command_Line;

Procedure Get_A_Command_Line(cmdline : out String; Last : out Natural) is 
-- This is an iterative way to return a fixed string type.  It first loops to count how much length is needed,
-- and then loops again to assemble the pieces, all using a Declare block, only fixed strings, and slices.
-- Credit to Brian Drummond.
  len : Natural := 0;

BEGIN  
  IF Argument_Count = 0 THEN
    cmdline := "";
    Last := 0;
    Return;
  END IF;
  FOR I in 1 .. Argument_Count LOOP
    len := len + argument(I)'Length;
  END LOOP;
  len := len + Argument_Count;  -- to also count the spaces including the one at the end.  I'm not sure what will happen to that one later.
  DECLARE
    result : String(1..len);
    posn : Natural := 1;
  Begin
    FOR I in 1 .. Argument_Count LOOP
      result(posn .. posn+Argument(I)'Length) := Argument(I) & ' ';
      posn := posn + Argument(I)'Length + 1;  --the +1 factor is to account for the space.
    END LOOP;
    Move(result,cmdline);
    Last := len;
  END; -- declare block
END Get_A_Command_Line;

Function Get_Command_Line return Unbounded_String is
-- This is also an iterative way, but it uses unbounded strings for the intermediate parts.  Credit to Jeffrey Carter.
--
  result : Unbounded_String := Null_Unbounded_String;

BEGIN
--  If Argument_Count = 0 THEN
--    Return "";
--  END IF; 
  FOR I in 1 .. Argument_Count LOOP
    result := result & Argument(I);
    IF I < Argument_count THEN
      result := result & ' ';
    END IF;  -- I < argcnt to append a space
  END LOOP; -- For I loop
  Return result;
END Get_Command_Line;

Function Get_Command_Line return String is
-- This is also an iterative way, but it uses unbounded strings for the intermediate parts.  Credit to Jeffrey Carter.
--
  result : Unbounded_String := Null_Unbounded_String;

BEGIN
  If Argument_Count = 0 THEN
    Return "";
  END IF; 
  FOR I in 1 .. Argument_Count LOOP
    result := result & Argument(I);
    IF I < Argument_count THEN
      result := result & ' ';
    END IF;  -- I < argcnt to append a space
  END LOOP; -- For I loop
  Return To_String(result);
END Get_Command_Line;

PROCEDURE GetCommandLine(CommandLine : OUT Str255) is
BEGIN
  n := argument_count;
  k := 1;
  cmdlinefrag := NullStr255;
  WHILE k <= n LOOP
    argstr := To255(argument(k));
    cmdlinefrag := cmdlinefrag & argstr;
    IF k < n THEN 
      cmdlinefrag := cmdlinefrag & ' ';
    END IF;
    k := Natural'SUCC(k);
  END LOOP; -- while k < n
  CommandLine := cmdlinefrag;
END GetCommandLine;

END environa;
