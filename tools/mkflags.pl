#!/usr/bin/perl
#
#  Copyright (C) 1997-2011 by Suntail.com AS, Tim Whelan
#
#  Permission is hereby granted, free of charge, to any person
#  obtaining a copy of this software and associated documentation files
#  (the "Software"), to deal in the Software without restriction,
#  including without limitation the rights to use, copy, modify, merge,
#  publish, distribute, sublicense, and/or sell copies of the Software,
#  and to permit persons to whom the Software is furnished to do so,
#  subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be
#  included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
#  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
#  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

use Getopt::Std;

getopt( "oih" );

$output = $opt_h;
$input = $opt_i;
$outcpp = $opt_o;

if ( $output eq "" ) {
  die "No output file specified";
}
if ( $input eq "" ) {
  die "No input file specified";
}
if ( $outcpp eq "" ) {
  die "No cpp output file specified";
}

open( INPUT, $input ) or die "Could not open $input for reading";
open( OUTPUT, ">$output" ) or die "Could not open $output for writing";

$curenum = "";
$curflags = "";

# Enum lists
addheader_enum();
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_enum( $_ );
    } else {
      if ( /^end:/ ) {
        processEnd_enum( $_ );
      } else {
        # it's a enum value
        processValue_enum( $_ );
      }
    }
  }
}

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";

addheader_decl();
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_decl( $_ );
    }
  }
}

close( OUTPUT );
open( OUTPUT, ">$outcpp" ) or die "Could not open $outcpp for writing";

# Create the .cpp implementation file

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

# Normal translations (A)
addheader_trans();
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_trans( $_ );
    } else {
      if ( $curflags =~ /A/ ) {
        if ( /^end:/ ) {
          processEnd_trans( );
        } else {
          # it's a enum value
          processValue_trans( $_ );
        }
      }
    }
  }
}

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

# list translations (C)

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_list( $_ );
    } else {
      if ( $curflags =~ /C/ ) {
        if ( /^end:/ ) {
          processEnd_list( );
        } else {
          # it's a enum value
          processValue_list( $_ );
        }
      }
    }
  }
}

# Add the isValid implementations

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_isValid( $_ );
    } else {
      if ( $curflags =~ /H/ ) {
        if ( /^end:/ ) {
          processEnd_isValid( );
        } else {
          # it's a enum value
          processValue_isValid( $_ );
        }
      }
}
  }
}

# Add the translate_DESC implementations (G)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_transDESC( $_ );
    } else {
      if ( $curflags =~ /G/ ) {
        if ( /^end:/ ) {
          processEnd_transDESC( );
        } else {
          # it's a enum value
          processValue_transDESC( $_ );
        }
      }
    }
  }
}

# Add the translate2 implementations (B)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_trans2( $_ );
    } else {
      if ( $curflags =~ /B/ ) {
        if ( /^end:/ ) {
          processEnd_trans2( );
        } else {
          # it's a enum value
          processValue_trans2( $_ );
        }
      }
    }
  }
}

# Add the translate_*_rev implementations (D)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_trans_rev( $_ );
    } else {
      if ( $curflags =~ /D/ ) {
        if ( /^end:/ ) {
          processEnd_trans_rev( );
        } else {
          # it's a enum value
          processValue_trans_rev( $_ );
        }
      }
    }
  }
}

# Add the translate_*_rev_desc implementations (E)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_trans_rev_desc( $_ );
    } else {
      if ( $curflags =~ /E/ ) {
        if ( /^end:/ ) {
          processEnd_trans_rev_desc( );
        } else {
          # it's a enum value
          processValue_trans_rev_desc( $_ );
        }
      }
    }
  }
}

# Add the translate_*_rev_list_desc implementations (F)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_trans_rev_list_desc( $_ );
    } else {
      if ( $curflags =~ /F/ ) {
        if ( /^end:/ ) {
          processEnd_trans_rev_list_desc( );
        } else {
          # it's a enum value
          processValue_trans_rev_list_desc( $_ );
        }
      }
    }
  }
}

# Add the translate_*_list_desc implementations (I)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_list_desc( $_ );
    } else {
      if ( $curflags =~ /I/ ) {
        if ( /^end:/ ) {
          processEnd_list_desc( );
        } else {
          # it's a enum value
          processValue_list_desc( $_ );
        }
      }
    }
  }
}

# Add the translate_*_all_list_desc implementations (J)

close( INPUT );
open( INPUT, $input ) or die "Could not open $input for reading";
$curenum = "";
$curflags = "";

print OUTPUT "\n";
while ( <INPUT> ) {
  $_ =~ s|\#.*||;
  if ( $_ ne "\n" ) {
    if ( /begin:/ ) {
      processBegin_all_list_desc( $_ );
    } else {
      if ( $curflags =~ /J/ ) {
        if ( /^end:/ ) {
          processEnd_all_list_desc( );
        } else {
          # it's a enum value
          processValue_all_list_desc( $_ );
        }
      }
    }
  }
}


close( INPUT );
close( OUTPUT );
exit;

sub processValue_isValid( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    print OUTPUT "$enum, ";
  }

sub processValue_list( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    if ( ! ( $desc =~ /\*/ ) ) {
      print OUTPUT "    if ( flags & $enum )\n";
      print OUTPUT "    {\n";
      print OUTPUT "        if ( OFOS::strlen( listof ) ) OFOS::strcat( listof, \", \" );\n";
      print OUTPUT "            OFOS::strcat( listof, \"$enum\" + (trim?OFOS::strlen(head)+1:0) );\n";
      print OUTPUT "    }\n";
    }
  }


sub processValue_all_list_desc( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    chomp $desc;
    if ( ! ( $desc =~ /\*/ ) ) {
      print OUTPUT "$desc,";
    }
  }

sub processValue_list_desc( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    chomp $desc;
    if ( ! ( $desc =~ /\*/ ) ) {
      print OUTPUT "    if ( enumValue & $enum )\n";
      print OUTPUT "    {\n";
      print OUTPUT "        if ( OFOS::strlen( listof ) ) OFOS::strcat( listof, \", \" );\n";
      print OUTPUT "            OFOS::strcat( listof, \"$desc\" );\n";
      print OUTPUT "    }\n";
    }
  }

sub processValue_trans( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    print OUTPUT "        { $enum, (char*)\"$enum\" },\n";
  }

sub processValue_trans2( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    print OUTPUT "        { $enum, (char*)\"$enum\" },\n";
  }

sub processValue_trans_rev( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    print OUTPUT "        { $enum, (char*)\"$enum\" },\n";
  }

sub processValue_trans_rev_desc( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    chomp $desc;
    print OUTPUT "        { $enum, (char*)\"$desc\" },\n";
  }

sub processValue_trans_rev_list_desc( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    chomp $desc;
    print OUTPUT "        if ( !OFOS::stricmp( \"$desc\", token ) )\n";
    print OUTPUT "            enumValue |= $enum;\n";
  }

sub processValue_transDESC( )
  {
    ( $enum, $val, $desc ) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    chomp $desc;
    print OUTPUT "        { $enum, (char*)\"$desc\" },\n";
  }

sub processValue_enum( )
  {
    ($enum, $val, $desc) = split( /:/, shift( @_ ) );
    if ($enum eq "") {
      return;
    }
    if (! $start) {
        print OUTPUT ",\n\n";
    }
    print OUTPUT "        /** $desc         */\n";
    print OUTPUT "        $enum";
    if ($val ne "") {
      print OUTPUT " = $val";
    }
    $start = 0;
  }

sub processEnd_isValid( )
  {
    print OUTPUT "    (ofuint32)-1 };\n";
    print OUTPUT "    bool found = false;\n";
    print OUTPUT "    for ( ofuint32 cur = 0; !found && allvalues[cur] != (ofuint32)-1; cur++)\n";
    print OUTPUT "        if ( enumValue == allvalues[cur] )\n";
    print OUTPUT "            found = true;\n";
    print OUTPUT "    return found;\n";
    print OUTPUT "}\n";
  }

sub processEnd_list()
  {
    print OUTPUT "}\n\n";
  }

sub processEnd_all_list_desc()
  {
    print OUTPUT "\";\n}\n\n";
  }

sub processEnd_list_desc()
  {
    print OUTPUT "}\n\n";
  }

sub processEnd_transDESC()
  {
    print OUTPUT "        { -1, (char*)\"END\" }\n";
    print OUTPUT "    };\n";
    print OUTPUT "    const char *descrip = 0;\n";
    print OUTPUT "    for ( ofuint32 x = 0; !descrip && names[x].index != -1; x++ )\n";
    print OUTPUT "        if ( names[x].index == (ofint32)enumValue )\n";
    print OUTPUT "        {\n";
    print OUTPUT "            descrip = names[x].name;\n";
    print OUTPUT "        }\n";
    print OUTPUT "    return descrip;\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_trans()
  {
    print OUTPUT "        { -1, (char*)\"END\" }\n";
    print OUTPUT "    };\n";
    print OUTPUT "    RESETSTRING( enumName );\n";
    print OUTPUT "    char found = 0;\n";
    print OUTPUT "    for ( ofuint32 x = 0; !found && names[x].index != -1; x++ )\n";
    print OUTPUT "        if ( names[x].index == (ofint32)enumValue )\n";
    print OUTPUT "        {\n";
    print OUTPUT "            found = 1; OFOS::strcpy( enumName, names[x].name );\n";
    print OUTPUT "        }\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_trans2()
  {
    print OUTPUT "        { -1, (char*)\"END\" }\n";
    print OUTPUT "    };\n";
    print OUTPUT "    const char *val = 0;\n";
    print OUTPUT "    for ( ofuint32 x = 0; !val && names[x].index != -1; x++ )\n";
    print OUTPUT "        if ( names[x].index == (ofint32)enumValue )\n";
    print OUTPUT "        {\n";
    print OUTPUT "            val = names[x].name;;\n";
    print OUTPUT "        }\n";
    print OUTPUT "    return val;\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_trans_rev()
  {
    print OUTPUT "        { -1, (char*)\"END\" }\n";
    print OUTPUT "    };\n";
    print OUTPUT "    ofuint32 index = 0;\n";
    print OUTPUT "    for ( ofuint32 x = 0; !index && names[x].index != -1; x++ )\n";
    print OUTPUT "        if ( !OFOS::stricmp( names[x].name, enumName) )\n";
    print OUTPUT "        {\n";
    print OUTPUT "            index = names[x].index;\n";
    print OUTPUT "        }\n";
    print OUTPUT "    return index;\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_trans_rev_desc()
  {
    print OUTPUT "        { -1, (char*)\"END\" }\n";
    print OUTPUT "    };\n";
    print OUTPUT "    ofuint32 index = 0;\n";
    print OUTPUT "    for ( ofuint32 x = 0; !index && names[x].index != -1; x++ )\n";
    print OUTPUT "        if ( !OFOS::stricmp( names[x].name, enumName) )\n";
    print OUTPUT "        {\n";
    print OUTPUT "            index = names[x].index;\n";
    print OUTPUT "        }\n";
    print OUTPUT "    return index;\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_trans_rev_list_desc()
  {
    print OUTPUT "    }\n";
    print OUTPUT "    return enumValue;\n";
    print OUTPUT "}\n\n";
  }

sub processEnd_enum()
  {
    print OUTPUT "\n    };\n\n";
  }

sub processBegin_transDESC( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /G/ ) {
      print OUTPUT "/* G */\n";
      print OUTPUT "const char *\ntranslate_${curenum}_desc( ofuint32 enumValue )\n";
      print OUTPUT "{\n";
      print OUTPUT "    struct NAMEPAIR { ofint32 index; char *name; };\n";
      print OUTPUT "    NAMEPAIR names[] = {\n";
    }
  }

sub processBegin_isValid( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /H/ ) {
      print OUTPUT "/* H */\n";
      print OUTPUT "bool\nisvalid_${curenum}( ofuint32 enumValue )\n";
      print OUTPUT "{\n";
      print OUTPUT "    ofuint32 allvalues[] = { ";
    }
  }

sub processBegin_list( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /C/ ) {
      print OUTPUT "/* C */\n";
      print OUTPUT "void\ntranslate_${curenum}_list( ofuint32 flags, char *listof, char trim /* = 0*/ )\n";
      print OUTPUT "{\n";
      print OUTPUT "    RESETSTRING( listof );\n";
      print OUTPUT "    const char *head = \"$curenum\";\n";
    }
  }

sub processBegin_all_list_desc( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /J/ ) {
      print OUTPUT "/* J */\n";
      print OUTPUT "const char *\ntranslate_${curenum}_all_list_desc()\n";
      print OUTPUT "{\n";
      print OUTPUT "    return \"";
    }
  }

sub processBegin_list_desc( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /I/ ) {
      print OUTPUT "/* I */\n";
      print OUTPUT "void\ntranslate_${curenum}_list_desc( ofuint32 enumValue, char *listof)\n";
      print OUTPUT "{\n";
      print OUTPUT "    RESETSTRING( listof );\n";
    }
  }

sub processBegin_decl( )
  {
    ( $dummy, $curenum, $desc, $funcflags ) = split( ":", shift( @_ ) );
    print OUTPUT "/* $curenum\n*/\n\n";
    if ( $funcflags =~ /A/ ) {
      print OUTPUT "void OFSYS_API translate_${curenum}( ofuint32 enumValue, char *enumName );/*A*/\n";
    }
    if ( $funcflags =~ /B/ ) {
      print OUTPUT "OFSYS_API const char * translate_${curenum}2( ofuint32 enumValue );/*B*/\n";
    }
    if ( $funcflags =~ /C/ ) {
      print OUTPUT "void OFSYS_API translate_${curenum}_list( ofuint32 flags, char *listof, char trim = 0 );/*C*/\n";
    }
    if ( $funcflags =~ /D/ ) {
      print OUTPUT "ofuint32 OFSYS_API translate_${curenum}_rev( const char *enumName );/*D*/\n";
    }
    if ( $funcflags =~ /E/ ) {
      print OUTPUT "ofuint32 OFSYS_API translate_${curenum}_rev_desc( const char *enumName );/*E*/\n";
    }
    if ( $funcflags =~ /F/ ) {
      print OUTPUT "ofuint32 OFSYS_API translate_${curenum}_rev_list_desc( const char *listof );/*F*/\n";
    }
    if ( $funcflags =~ /G/ ) {
      print OUTPUT "OFSYS_API const char * translate_${curenum}_desc( ofuint32 enumValue );/*G*/\n";
    }
    if ( $funcflags =~ /H/ ) {
      print OUTPUT "bool OFSYS_API isvalid_${curenum}( ofuint32 enumValue );/*H*/\n";
    }
    if ( $funcflags =~ /I/ ) {
      print OUTPUT "void OFSYS_API translate_${curenum}_list_desc( ofuint32 enumValue, char *listof );/*I*/\n";
    }

    print OUTPUT "\n";
  }

sub processBegin_trans( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /A/ ) {
      print OUTPUT "/* A */\n";
      print OUTPUT "void\ntranslate_${curenum}( ofuint32 enumValue, char *enumName )\n";
      print OUTPUT "{\n";
      print OUTPUT "    struct NAMEPAIR { ofint32 index; char *name; };\n";
      print OUTPUT "    NAMEPAIR names[] = {\n";
    }
  }

sub processBegin_trans2( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /B/ ) {
      print OUTPUT "/* B */\n";
      print OUTPUT "const char *\ntranslate_${curenum}2( ofuint32 enumValue )\n";
      print OUTPUT "{\n";
      print OUTPUT "    struct NAMEPAIR { ofint32 index; char *name; };\n";
      print OUTPUT "    NAMEPAIR names[] = {\n";
    }
  }

sub processBegin_trans_rev( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /D/ ) {
      print OUTPUT "/* D */\n";
      print OUTPUT "ofuint32\ntranslate_${curenum}_rev( const char *enumName )\n";
      print OUTPUT "{\n";
      print OUTPUT "    struct NAMEPAIR { ofint32 index; char *name; };\n";
      print OUTPUT "    NAMEPAIR names[] = {\n";
    }
  }

sub processBegin_trans_rev_desc( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /E/ ) {
      print OUTPUT "/* E */\n";
      print OUTPUT "ofuint32\ntranslate_${curenum}_rev_desc( const char *enumName )\n";
      print OUTPUT "{\n";
      print OUTPUT "    struct NAMEPAIR { ofint32 index; char *name; };\n";
      print OUTPUT "    NAMEPAIR names[] = {\n";
    }
  }

sub processBegin_trans_rev_list_desc( )
  {
    ( $dummy, $curenum, $desc, $curflags ) = split( ":", shift( @_ ) );
    if ( $curflags =~ /F/ ) {
      print OUTPUT "/* F */\n";
      print OUTPUT "ofuint32\ntranslate_${curenum}_rev_list_desc( const char *listof )\n";
      print OUTPUT "{\n";
      print OUTPUT "    ofuint32 enumValue = 0;\n";
      print OUTPUT "    ofuint32 start = 0;\n";
      print OUTPUT "    char token[129];\n";
      print OUTPUT "    while ( OFUtility::str_token2( listof, \",\", \"\$\$\", &start, token, 128, 1 ) )\n";
      print OUTPUT "    {\n";
    }
  }

sub processBegin_enum()
  {
    my $line = shift( @_ );
    ( $dummy, $curenum, $desc, $funcflags ) = split( ":", $line );
    print OUTPUT "/** \\enum $desc */\n";
    print OUTPUT "enum $curenum\n    {\n";
    $start = 1;
  }

sub addheader_decl( )
  {
    print OUTPUT "\n\n";
    print OUTPUT "/*\nThe following functions are generated for each enum:\n\n";
    print OUTPUT "Convert the supplied enumValue to the text name\n";
    print OUTPUT "A    void translate_ENUM( ofuint32 enumValue, char *enumName );\n\n";

    print OUTPUT "Same as above, but returns the enum text value\n";
    print OUTPUT "B    const char * translate_ENUM2( ofuint32 enumValue );\n\n";

    print OUTPUT "Converts a bitmask of enum values into a list containing their text values\n";
    print OUTPUT "Includes an optional trim of the value (removes the first X chars from the text value, \n";
    print OUTPUT "where X is the number of characters in the enum name\n";
    print OUTPUT "C    void translate_ENUM_list( ofuint32 flags, char *listof, char trim = 0 );\n\n";

    print OUTPUT "Performs a reverse lookup, given the text value, returns the enum value.\n";
    print OUTPUT "D    ofuint32 translate_ENUM_rev( const char *enumName );\n\n";

    print OUTPUT "Same as above, but performs the lookup on the enum description.\n";
    print OUTPUT "E    ofuint32 translate_ENUM_rev_desc( const char *enumName );\n\n";

    print OUTPUT "\n";
    print OUTPUT "F    void translate_ENUM_rev_list_desc( ofuint32 flags, char *listof );\n\n";

    print OUTPUT "Retrieves the enum description for a enum value\n";
    print OUTPUT "G    const char * translate_ENUM_desc( ofuint32 enumValue );\n\n";

    print OUTPUT "Checks if a supplied enum value is valid.\n";
    print OUTPUT "H    bool OFSYS_API isvalid_${curenum}( ofuint32 enumValue );\n";

    print OUTPUT "Translates a bitmask (list) enum value to a description list.\n";
    print OUTPUT "I    void translate_ENUM_list_desc( ofuint32 enumValue, char *listof );\n";

    print OUTPUT "\n\n*/\n";
  }

sub addheader_trans( )
  {
    print OUTPUT "/** This file is generated automatically. Do not modify it.\n */\n";
    print OUTPUT "#include <ofsys.h>\n";
    print OUTPUT "\n";
  }

sub addheader_enum()
  {
    print OUTPUT 
      "/* This file was automatically generated. Do not modify it.\n",
        "*/\n";

    print OUTPUT
      "\n/** \\file $output\n    Contains the generated flag enumerations\n */\n\n";

  }
