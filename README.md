# Simple Media System

Source code to Simple Media System, originally designed by Eugene Plotnikov. SMS is capable of playing DivX/XviD videos on PS2 very well, along with MP3/FLAC audio. It supports loading from USB, DVD, SMB and HDD, and it comes equipped with features such as built-in 720p/1080i support and its own data disc storage format (CDDAFS).

NOTE: master branch will have the original source code. I will have my own branch with modifications when/if I see fit.

### Original Readme

```PS2DEV Open Source Project.
---------------------------------------------------------
 Copyright 2001-2005, ps2dev - http://www.ps2dev.org
 All rights reserved.
 Created by Eugene Plotnikov <e-plotnikov@operamail.com>
-----------------------------------------------------------------------

Introduction
-----------------------------------------------------------------------
Simple Media System (SMS) is a result of my curiosity about Sony's Playstation2,
MIPS, DivX and MP3. My original goal was to create a simplest DivX player able
to play good resolution movies at good frame rate on the unmodded PS2 without
any extra equipment such as HDD and network adapter. Why "system"? Just because
having only a player program is not enough. Since PS2 is forced to ignore "illegal"
disks, I've took an idea of Drakonite with his UMCDR and developed my own "UMCDR".
(I called it CDDAFS). It just stores data on CD audio disk, which can be read on
(I hope) any PS2. So this is a first component of SMS. Second component is a PC
program that makes cue/bin files in order to burn such a disk. And, finally, comes
a player program itself. This system is not complete yet, but it can play DivX movies
with sound at good frame rate already. Player's base is a famous ffmpeg project.
I took just some parts of it (DivX 5XX and MP3) and made some modifications specific
to PS2 hardware. I've tested it with a couple of DivX/XviD movies with different
screen resolutions and encoding methods and it seems to be working. So, I've decided
to put all this stuff to the CVS at ps2dev in hope that it could be interesting for
PS2 enthusiasts who use it as base for their development.  This "document" provides
basic info about SMS.

 CDDAFS
 ----------------------------------------------------------------------
It's just a method of storing a data on audio CD, so PS2 hardware can authenticate
such a CD as "valid" one. Logical structure of such a CD is quite simple. Audio tracks
are "directories". Track 0 is a control track, which stores global disk information.
Following "diagram" presents a logical disk structure:

Track 0:
GUID             - 16  bytes
Disk name        - 64  bytes
Disk description - 512 bytes
Disk picture     - 9216 bytes (48x48x4 raw RGB image)*
; then follows image table
Number of images - 2 bytes (short, little endian)*
Image 1          - 4096 bytes (32*32*4 raw RGB image)*
.......
Image N          - 4096 bytes (32*32*4 raw RGB image)*
; then follows "directory" table (number of directories is number of tracks from
; CD TOC minus one)
Track name        1 - 32 bytes
Track image index 1 -  2 bytes (short, little endian, zero based)
Reserved          1 - 30 bytes (just for possible future enhancements)
Track name        2 - 32 bytes
Track image index 2 -  2 bytes (short, little endian, zero based)
Reserved          2 - 30 bytes (just for possible future enhancements)
.......
Other tracks:
; Directory entry 1
Number of files     -  2 bytes (short, little endian)
File name           - 32 bytes
File image index    -  2 bytes (short, little endian, zero based)
Reserved            - 30 bytes
Start offset        -  4 bytes (physical sector relative to start sector of
                                the current track, int, little endian, zero based
                               )
File size(in bytes) -  4 bytes (int, little endian)
; Directory entry 2
; etc.
; File data

* these "pictures" and "images" are in fact RGBA raw bitmaps, so future browser
  can be decorated with them;

Such a disk (without extra ECC codes) proved reliable enough in my configuration.
Source code that handles CD I/O located in CDDA.c (low level sector read operations)
and in FileContext.c (logical read operations). I've also started implementation of
"standard" I/O (STIO_xxx routines). Well, for me CDDA is enough. So anyone can
finish "standard" I/O (perhaps, using asynchronous fileXio routines from ps2sdk).

 PC program
 ----------------------------------------------------------------------
I've called it CDXplorer. Not fully completed yet, but it does its job of
making cue/bin pair. This program is uses Win32 API, so it is not compatible with
linux (even WINE has troubles running it). I'm using Nero's Disk-At-Once/96 burning
option. It is also possible to read CDDAFS using this proggy. Just insert CDDAFS disk
in the drive and run the program. Maybe someone would like to create fs driver to
support it :)?

 SMS Player
 ----------------------------------------------------------------------
Just a test program for now and PS2 optimized ffmpeg. No GUI. It is possible to
benchmark video/audio decoding process (I've got sometimes peak ~70 fps with
640x464 movie (~60fps is average)). I was able to play 720x528 DivX with sound
at 25fps (but in this case there're slight sound distortions on scenes with a
lot of motions (mainly "global" ones). Just watching a movie is also possible.
Modify main's source at your taste. For sound output I'm using 'audsrv' 0.75
(magnificent work of gawd).

 ffmpeg modifications
 ----------------------------------------------------------------------
Two things here. Using IPU for colorspace conversion (interrupt driven process that
performs CSC->Display sequence for 640x464 picture in just ~4ms). This process goes
in parallel with video/audio decoding. Second, I've made a transition from planar
image format to the packed one, so video decoder produces macroblocks in IPU format.
This notable reduced data traffic between main CPU and memory. SPR is also
heavily used in conjunction with DMA transfers. This things gave ~300% performance
boost (comparing to the original ffmpeg) in video decoding. MP3 decoder is virtually
unmodified.

 Things to do
 ----------------------------------------------------------------------
There still a lot to do. As I'm a professional programmer, I just can't do this
alone quickly. SMS is just a working skeleton of the future player. I've created it
in my own free time. The following things I'd like to do with it:
- more testing (I've tested it with just about a dozen of DivX/XviD movies);
- more optimizations (mainly for GMC decoding. I've made just an 'ad hoc' code to
  show that it works. But this code is ugly and slow). QPel works, but it is
  unoptimized too (just an original ffmpeg). The MP3 decoder from ffmpeg is also
  almost untouched;
- better A/V synchronization. In fact there's no synchronization at all.
  But because multithreading on PS2 is not preemptive it happens
  "automatically" with a surprisingly good accuracy :). The only thing to
  fix was initial "audio preload". I'm not sure if I made it correctly, but
  it seems to be working at the moment. Realtime clock with 1ms resolution is
  also implemented, so it could be used for more precise synchronization;
- creation of the GUI. In case of CDDAFS it would be just a disk browser.
  There's already a base for it. There're routines to use ROM font (FONTM) and display
  icons from CDDAFS disk. There're also routines to load directory/file lists.
  The only thing missing is a pad management and an event loop;
- implementation of pause/stop functionality; As movie starts, there's no way back now;
- error checking (there're minimal checks only, so if, for example, press
  'eject' CD button, then...a very interesting effect can be achieved :);
- btw, can somebody modify Makefile in order to make whole project more structured
  (all these 'ee', 'iop' etc. directories)? 

 Restrictions
 ----------------------------------------------------------------------
- no support for interlaced movies;
- no support for non-interleaved avi files;
- only CodecID_MPEG4 support (that is DivX 5XX/XviD (I never, however, tried
  these 'MP4S', 'BLZ0' etc. (what are they?)) and MP3;
- other possible restrictions due to lack of detailed knowledge about things :);

 How to compile
 ----------------------------------------------------------------------
I'm using latest toolchain/ps2sdk from ps2dev and Win32 platform. Just 'make'.
Do not use 'old' ps2sdk (earlier than january 2005), since there's a bug in
'realloc', so program crashes immediately. In my setup I use ps2link at PS2 side
and InLink at the PC. Note also that performace depends on PS2 cache usage,
so even changing link order in Makefile can get more than 10% penalty (
Currently Makefile contains 'best' link order (I think so). Pay attention to
the location of 'audsrv.irx' (currently 'host:audsrv.irx').

 How to debug
 ----------------------------------------------------------------------
Crazy enough, but I've made the code Win32 compatible (with exception of some
files). So, I'm using MSVC6 to debug major bugs, memory leaks and to understand
ffmpeg's code. 'printf' does debug for PS2 :);

 How to make modifications
 ----------------------------------------------------------------------
At your taste (be careful, any alignment failure causes a crash (especially with
structures allocated dynamically)). But first compile and run existing code (the only
thing to modify is a filename in 'main.c'). To do that it's necessary to waste at least
one CD-R for CDDAFS. Use CDXplorer application to make cue/bin pair. It is possible
to copy files back from CDDAFS disk. Drag-and-drop is supported (one direction only:
to the application). Context menus are also available. Number of directories has upper
limit of 99 (only one level), and number of files is restricted by the CD capacity
(I've put a 800MB movie, for example, on the ordinary 700MB disk without any problem).
I have SCPH-30004R PAL unit, so it works at least on this model.

 Modified ps2sdk code
 ----------------------------------------------------------------------
It could be noted that there's a lot of modified ps2sdk code here. Why?
I wanted maximum performance. For 'audsrv' for example there's an unnecessary
(I think so) calls for memcpy and FlushCache. 'libcdvd' caused strange problems
of data corruprion when SifLoadModule was called. GSkit uses SPR badly needed
for video decoding. Instead, I decode MP3 stream directly to the uncached, 64 byte
aligned memory, I'm using 64 byte aligned buffers for disk I/O, and GS kit was
modified to avoid SPR usage. Maybe I'm just a stupid person :o).

 License
 ----------------------------------------------------------------------
For my code it is the same as for ps2sdk (AFL). For ffmpeg -
"GNU Lesser General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version" :).

 Miscellaneous
 ----------------------------------------------------------------------
I've made this just for myself. I don't want to make any profit from it.
Anyone is free to make anything with it (according to aforementioned licenses).
If my code sucks (especially my formatting habits) - it sucks. I don't care.
I just hope that it will help PS2 enthusiasts to make their programs quicker.
I will continue slowly to make it more like a real system, but for me alone
it will take ages to finish it :).

 Questions
 ----------------------------------------------------------------------
- by email (see beginning of file);
- using private messages at ps2dev's forums;

 Thanks (in no particular order) ;o)
 ----------------------------------------------------------------------
- Sony for PS2 and Linux for it;
- Marcus R. Brown for discovering an exploit (without it I would never begin);
- Drakonite for UMCDR idea;
- Pixel for making an estimation of the code and for helping me out with CVS;
- J.F. for trying this out (btw. I don't know if you succeeded with it or not);
- PS2Reality team for inspiration and timer code;
- ffmpeg contributors;
- Chris "Neovanglist" Gilbert for GS and DMA code;
- gawd for 'audsrv' (virgin version was enough :));
- all others whose posts in the forums helped me in development;


Update (16-JUN-2005)
--------------------
STIO_XXX routines are implemented. Though not fully tested, but at least I was
able to play a file using 'host:file.avi' syntax. Loading goes slower than from
CDDAFS but movie plays OK. This feature can be helpful for programmers (no need
anymore to make CD :)). Well, these STIO_XXX routines call 'fioxxx' from ps2sdk,
so, alas, no hdd support. Anyone is free to replace 'fioxxx' by 'fileXioxxx' and
try it with hdd. To use 'STIO_XXX' just replace 'CDDA_InitFileContext' call by
'STIO_InitFileContext' (and remove other CDDA_XXX calls).

Update (19-JUN-2005)
--------------------
HDD support added. All requred .irx modules are loaded from memory (They reside in
SMS_Data.c (Note that data buffer is then reused by audio decompressor)).
HDD partition "SMS_Media" (FS_GROUP_COMMON) is mounted automatically. Finally fixed
ring buffers issue (the problem was main's thread priority (I thought it is 1 by default,
but 'ps2link' sets it to 64)). Now it explicitly set to 1. Changed audio decompression
buffer (I'm preparing A/V synchronization in such a way that there will be two more
queues with already decompressed audio/video frames (I'm not sure yet if it will work
at all :)). Audio preload functionality is removed (no need of it as my idea with A/V
sync will not depend on it). I don't know at the moment how USB devices work with ps2sdk,
so any help will be appreciated. Project structure is also reorganized.

Update (28-JUN-2005)
--------------------
Some MP3 bugs (typoes) fixed. New FONTM autokerning algorithm is implemented (strings look
much nicer now). 'audsrv' updated. Region autodetection code is added to the GS code.

Update (03-JUL-2005)
--------------------
A very primitive A/V syncronization added. Video output V-Blank synchronization added.

Update (15-JUL-2005)
--------------------
At last I've added a GUI and pause/stop functionality, so it looks more like a real program.
Here's short manual:
- SMS screen is divided in 3 areas: device menu at the top, file menu in the middle and
  status line at the bottom;
- each area is bounded by a red rectangle with round corners;
- device menu and file menu can be activated;
- active menu is bounded by a white rectangle;
- device menu is navigated by "left" and "right" pad buttons;
- "down" pad button switches from device menu to the file menu;
- file menu is navigated by "up" and "down" pad buttons;
- "left" or "right" pad button switches from file menu to the device menu;
- active item in both menus is highlighted by semitransparent rectangle;
- selection is made by pressing "cross" pad button;
- "triangle" pad button navigates one level up (directory->parent directory->partition list)
  in the file menu;
- menu items with different contexts (file/directory/partition) are represented by
  different icons;
- CDDAFS disks are recognized automatially (just insert it and wait for a while);
- during playback use "select" pad button to pause, "start" to resume and "triangle"
  to stop and return to the filebrowser;
- press "select" and "circle" pad buttons in the filebrowser to switch PS2 off;
That's it. Note, that during that "harsh stop action" no IOP reset, program restart etc.
is performed. So, memory and other resource leaks etc. are possible (I've tried to be
as accurate as possible but who knows ;)). No USB mass-stoarge device support, since my
only USB memory stick doesn't work with ps2sdk tests. Note also, that I've changed that
magic MAGV, MAGH and other numbers in GS.c to get nice aligned picture on my TV and TV
tuner card (SMS in NTSC mode also works there). If anyone has a problems with improperly
aligned picture - just play around with these numbers. VESA modes are not tested at all
(perhaps I'll try to play with it).

Another thing: as this become more or less complete version there could be a huge mess
with distributed versions. There are two choises here: maintain only one version or change
something to make a difference. I personally don't believe in choise 1. So, SVN will also
contain a precompiled SMS.elf (let's call it "reference executable"). There's a function
called 'GUIStub_DrawBackground' which is empty in SVN (in my case it draws a gradient
background, animation and version number (currently 1.0 Rev.2)). Different "distributors"
would implement their own "signature" by implementing aforementioned function
(its parameter gives an access to the GSContext etc.), so people will know who's
to blame ;)). Anyone is welcome to send me code modifications, so I'll update SVN.
There's still an optimization potential (GMC, MP3, for example) if someone thinks that
SMS is too slow. For audio issues, please, contact 'gawd', the creator of 'audsrv'
(32000 upsampler does not exist, for example). Better A/V sync could also be implemented
(the existing one is rather primitive (though, it works in most cases)).
There're a dosen of various DivX/XviD encoding parameters and their combinations etc.
I just could not handle them alone, sorry for that.

Update (20-JUL-2005)
--------------------
Fiexed bugs:
- movie stop action does not work always;
- movie freezes after some time when launched via LaunchElf;
- motion compenstation does not perform properly for some motion types;

New features:
- saved file browser state when selecting partition/directory;
- screen adjustments actions added:
    select-R1 - move right;
    select-L1 - move left;
    select-R2 - move down;
    select-L2 - move up;
- save configuration (select-SQUARE) action added ("mc0:/SMS" directory)
  (currntly screen settings and HDD mounted partition name);
- MP3 optimization added (assembly source from BroadQ) - audio decoding
  performance increased by ~30%;
- new version number - 1.1 ;);

Update (23-JUL-2005)
-------------------
- added "exit to the ps2 browser" action (select-TRIANGLE);
- tried to fix difficult-to-catch (thread stuff) "movie stop action does not work always";
- USB support added (thanks and regards to 'weltall'):
  USBD.IRX is not included (licensing issues). SMS looks for it in:
  - host:USBD.IRX
  - mc0:/BOOT/USBD.IRX
  - mc0:/PS2OSCFG/USBD.IRX
  - mc0:/SYS-CONF/USBD.IRX
  - mc0:/PS2MP3/USBD.IRX
  - mc0:/BOOT/PS2MP3/USBD.IRX
  - mc0:/SMS/USBD.IRX
  *** in aforementioned order ***;
  I'm using legal USBD.IRX from Playstation 2's Linux kit.

Note: there's modified usb_mass.irx integrated into SMS. I've made it in
order to get my USB flash drive (Apacer Handy Steno HT202 128MB USB2.0)
working. Hot swapping apparently works also in my case. Any remarks are
welcome. NO WARRANTY THAT USB FUNCTIONALITY WILL BEHAVE PROPERLY WITH OTHER DEVICES.

Update (31-JUL-2005)
--------------------
- another algorithm of A/V syncronization implemented;
- some bugs hopefully fixed :);
- video output V-Blank synchronization scheme changed;

Update (07-AUG-2005)
--------------------
- an attempt to fix a bug due to XviD's packed bitstream;
- translational global motion compensation routine is rewritten
  using EE's MMI. I'm not sure if it will be really noticeable
  but tests show that new routine is >4x faster than the original
  C code;

Update (13-AUG-2005)
--------------------
- got rid of the spontaneous noise (audsrv's problem);
- "player hangs on movie stop" bug fixed (thanks to 'Mr. Worm~~' for being
  patient with testing);
- got rid of "Paused" message by holding "select" button for > 200ms;
- performance slightly improved for some cases (avoiding calculations
  of skipped macroblocks);
- added video mode selection at program's startup
  (select-R1 for NTSC, select-R2 for PAL). Timeout is 1 second, then
  (if no aforementioned pad button combinations were pressed)
  program continues with default video mode (from saved configuration or
  autodetected one);

Update (19-AUG-2005)
--------------------
- AC3 audio decoder added. Absolutely unoptimized yet (just an original
  C code) but at least appears to be working (thanks to 'Mr. Worm~~' for
  his test video clips);
- Filebrowser's font color changed to yellow (an idea of 'Mr. Worm~~' again);
- 'DSP_PutPixels8XY2' and 'DSP_PutPixels16XY2' routines are rewritten in
  assembly using MMI;
- Some other changes that could be interesting only for programmers;

Update (26-AUG-2005)
--------------------
- Ronald Andersson (AKA: dlanor) has added network (host) support
  (thanks and regards to him):
- ISO CD/DVD support added (thanks to 'Mr. Worm~~' for real help in testing);
- Added MPEG2 unquantizer (fixes bad video quality issues in some cases)
  (this thing is to be optimized with MMI, anyone can try his (her) skills
   with MIPS/EE assembly ;) (thanks to 'koki373737' for his video clips);
- Added an ability to choose between interlaced and non-interlaced video
  signal at program's startup (select-R1-square - for non-interlaced NTSC
  and select-R2-square for non-intrlaced PAL);
- Attempt to fix "movie dimensions must be dividable by 16" issue. I've only
  tested it with height of 396 pixels, but I hope that it will work in other
  cases too (thanks to 'koki373737' again for his video clips);

Update (02-SEP-2005)
--------------------
- 'usb_mass.irx' updated (thanks to 'hip203' for source code of 'LaunchElf');
- AC3 sound is made louder (thanks to 'Mr. Worm~~' for playing with it);
- slightly improved performance ("CSC - display" cycle (IPU code) is shortened
  and some code is rewritten in assembly);
- removed "black bars" on the left and right sides of the picture for some movies
  with "incorrect" aspect ratios;

Update (21-SEP-2005)
--------------------
- DVD video disks support added (an ability to read .avi files from DVD video disks
  on "unmodded" consoles). New CDVD.IRX (supporting UDF filesystem) allows that.

Update (29-SEP-2005 "Birthday edition" :))
--------------------
- Added support for following codecs:
  'DIV3', 'MP43', 'MPG3', 'DIV5', 'DIV6', 'DIV4', 'AP41', 'COL1', 'COL0';
- Slightly changed 'audsrv' (an attempt to solve spontaneous "hangings");
- Some changes in the source code to prepare OSD support etc.;

Update (07-OCT-2005)
--------------------
- Changed/rewritten 'audsrv':
    - removed CDDA stuff (not used);
    - reduced data traffic;
    - upsamplers rewritten in assembler;
  Result: improved performance -> no sound crackling during network playback;
- Volume control is implemented (up/down buttons);
- AVI demuxer changed (OpenDML support (not tested - I don't have any .avi ot that type));
- General demuxer code is redesigned (this can make life easier for programmers
  who want to implement different file formats support);

Update (15-OCT-2005)
--------------------
- Fixed bug in cdvd.irx/UDF filesystem code;
- New audio upsampling routine is implemented. It employs linear interpolation
  algorithm and it still works at the IOP side (it uses no division instructions).
  It sould also support other sample rates (at least 32000 does work at my site);

Update (30-OCT-2005)
--------------------
- Attempt to fix misterious SifRpc/fioRead problem (decreased buffer size);
- Yet another A/V syncronization algorithm is implemented;
- Fixed a typo bug caused really bad A/V sync in some cases;
- Added movie "scrolling" actions:
  - "right"    pad button - "scroll" forward;
  - "left"     pad button - "scroll" backward;
  - "cross"    pad button - terminate "scrolling" and continue playing;
  - "triangle" pad button - terminate "scrolling" and return to the browser;
  This "scrolling" begins at 3 seconds intervals looking for closest keyframe
  which is displayed (so 3 seconds is only a hint, it can become, for example,
  20 or more/less actual seconds depending on keyframe locations). Pressing
  "left"/"right" while "scrolling" increases/ decreases intervals by 3 seconds
  (for forward "scrolling" "right" button increases interval and "left" button
  decreases it, for backward "scrolling" it's the other way around). Minimum
  value for the "scrolling" interval is 3 sec. Maximum is 1 min and 3 sec. If
  "scrolling" reaches begin of the movie then it begins to play from the beginning.
  Reaching end of the movie causes exit to the browser. Reaction time to pad
  buttons depends on the media speed etc., so sometimes it's necessary to hold
  buttons for a while for the response. At the beginning of "scrolling" and at the
  end of it some pause can happen (flush file buffers and termination of pending
  I/O requests) which again depends on the device speed. It is also possible to
  "wrap" scrolling process, for example, if while "scrolling" forward "left" button
  was pressed at the minimum "scrolling" interval then backward "scrolling" begins
  and vise versa. Note, that during "scrolling" image distortion can occur ("square"
  arifacts etc., I don't have any interest to investigate reasons for it :) ).
  Anyone is free to implement more advanced seeking functionality with time scale,
  "mouse" pointers etc. Current implementation permits "jumps" to arbitrary movie
  location (time based), so it can be easly reused for these "advanced" actions;
- added support for second gamepad. Thanks to 'bix64' for research and
  implementation;
- added very basic support for background image (skin) in the browser. Thanks to
  'bix64' for research and implementation. Skin file is a DivX5xx/XviD encoded
  I-Frame. It must be "mc0:SMS/skin.sms" file and it must have 640x512 dimensions;

Update (12-NOV-2005)
--------------------
- new font is introduced. It is internationalized and
  there's a possibility to choose "codepages". Four "codepages" are available:
  WinLatin1, WinLatin2, WinCyrillic and WinGreek.
  Thanks to "Mr.Worm~~" for the cool font utility;
- SMS menu is implemented ("start" pad button) providing configuration options etc.
  Thanks to "bix64" for the code, "Mr.Worm~~", "rathunter" and "HypERSoniC" for ideas;
  Note on color selection: to make life easier for me :) I've made a 16 color palette
  (standard CGA colors), so there's a color choice from these 16 colors. It's possible
  to customize this palette by providing "mc0:SMS/SMS.pal" file. It should be a binary
  file of 64 bytes length. Each color takes 4 bytes (R,G,B,A);
- Fixed small issue with "Initializing SMS..." message when there's no media
  available (thanks to "jacky401" and "Mr. Worm~~" for note and research);

Update (24-DEC-2005 "Christmas edition" :))
--------------------
- subtitles support is added for .srt (SubRip) and .sub (MicroDVD) formats. Subtitle
  files are recognized automatically by file extension (".srt" has precedence over
  ".sub") as they are located at the same place as .avi file and have the same name.
  Subtitles can be switched on/off via player menu. Note that this support is rather
  strict in sense that no advanced text parsing was implemented, so subtitle file must
  follow specifications (for .srt refer http://www.matroska.org). In case of "Subtitle
  error" message refer displayed number as line number near the place where error
  condition was detected. If subtitle line is too long to fit the screen it will be
  splitted (by word) in two or more lines (space character is treated as word separator).
  Thanks to "BraveDog" for the research/implementation, "Mr.Worm~~" for advises
  and testing and "mastaalien" for remark about long subtitle lines;
- player menu is introduced ("start" button during playback);
- multi-audio support is added (if multiple sudio streams are present in the .avi file).
  Note that all audio streams have to be encoded using same codec and same codec settings,
  otherwise results are unpredictable :). Language can be changed via player menu;
  Thanks to "Mr.Worm~~" for sample clip;
- SMS menu is extended with new options ("circle" pad button can also be used for action
  (for numeric input it reverses "scrolling" direction));
- external font support added. Following fonts can be loaded:
  - mc0:SMS/ascii.mtf    - contains  96 ascii    characters ( 32-127)
  - mc0:SMS/latin2.mtf   - contains 128 latin2   characters (128-255)
  - mc0:SMS/cyrillic.mtf - contains 128 cyrillic characters (128-255)
  - mc0:SMS/latin1.mtf   - contains 128 latin1   characters (128-255)
  - mc0:SMS/greek.mft    - contains 128 greek    characters (128-255)
  New font can be created by 'MTKFontCreator' utility (google) (I've used version 0.6.2).
  During font save select "Save Mediatek Font -> 4 color comp., variable width (NEW mode)".
  Min height must be set to 32. During font generation Max Width and Height parameters
  (displayed on the status line) must not exceed 32. No error checks etc. are performed
  by SMS here, so use it very carefully and on your own risk :). Probably .ini file
  for that utility has to be adjusted on site in order to generate reduced character sets
  (96 and 128 characters instead of 256). By changing that .ini file it's possible to
  create virtually any character sets (ISO-8859-7, for instance);
- pan-scan display is implemented. By default SMS displays movie in letterbox mode
  (widescreen movies are shown in full width). 3 pan-scan modes are available (via player
  menu or "square" pad button during playback). In pan-scan mode it's possible to scroll
  picture left or right using "L1"/"R1" pad buttons ("L1"+"R1" resets changes);
- "about" screen is added ("L1"+"R1"+"R2"+"L2" pad buttons in the browser);
- ps2host.irx driver is updated (fixed "DVD hang" issue on some consoles);
- fixed bug (only one file can be opened) in cdvd.irx;
- playback time OSD added ("cross" button during playback);
- auto poweroff timer added (start counting from the moment of pad button press).
  "auto" mode there means "poweroff when movie ends";
- fixed bug "mounted partition saved in the configuration when there's no partition
  mounted. Thanks to JuddMan for the remark;
- audio/video and subtitle/video manual synchronization added. "circle" pad button
  activates OSD for it and "L2"/"R2" adjusts time delay (from -5sec to 5sec (for
  subtitles from -30sec to 30sec) with 250ms interval);
- basic and primitive mp3/m3u playback support added with silly "ballsim" demo :);
- bug fixed (sceen offset adjustment settings is saved incorrectly) - thanks to
  'dave_t' for the fix;
- video timeline/scrollbar added. This option is configurable and activated by "select"
  pad button during playback (instead of pause). The use "left"/"right" pad buttons
  to set new time offset, "start"/"cross" pad button(s) to resume playback. Thanks to
  bix64 for the implementation;
- enabled support of VESA video modes: 640x480@60Hz ("select+L1" pad buttons at startup)
  and 640x480@75Hz ("select+L2" pad buttons at startup). Tested only with ViewSonic VX910
  TFT monitor with official Sony's cable. These modes are not selectable via SMS menu to
  avoid confusions :);
- fixed bug upsampler routine ("ticking" noise with sample rates < 48KHz);
- new bugs have been "implemented" ;);

Update (31-DEC-2005 "New Year edition" :))
--------------------
- fixed "hang" bug during m3u playback (thanks to "Mr.Worm~~" for nice Spanish
  music);
- fixed "subtitles are out of sync after scroll operations (using scrollbar)" bug (thanks
  to "Mr.Worm~~" for the remark);
- fixed "hang on loading .sub format subtitles" bug (thanks to "LaptoniC" for the remark);
- opaque subtitles support is added (thanks to "HUB107" for the note);
- added "support" for ".SRT" and ".SUB" (capital letters) subtitles;
- slightly changed AC3 decoder to get rid of some "effects" (thanks to "firehawk" for
  the research);
- removed garbage at the bottom of screen;
- added "fullscreen display" player option (thanks to "deank" for the note);
- fixed "movie jumps backward when resuming from pause using scrollbar" bug (thanks to
  "Mr.Worm~~" for the remark);

Update (01-JAN-2006)
--------------------
- fixed "first letter is missing" for "sub" subtitles format bug (thanks to
  "LaptoniC" and "deank" for remarks);
- fixed "SMS crashes when an unsupported file is selected" (thanks to "deank"
  for the remark);
- added "save display mode (letterbox/pan-scan)" feature; (thanks to "deank"
  for the remark);

Update (16-JAN-2006)
--------------------
- fixed bug in MP3 decoder - sound quality is improved (thanks to "Mr.Worm~~"
  and "Voldemar_u2" for testing);
- another attempt has been made to fix hang during m3u playback/scroll;
- relaxed format interpretation for .srt subtitles. Thanks to "Voldemar_u2"
  for the implementation;

Update (19-FEB-2006)
--------------------
- GS related code is rewritten from scratch (no VESA video mode support yet);
- Browser and GUI are rewritten (same look and feel, but totally different code
  featuring "object oriented" desktop, so, new GUI objects can be added without
  changing other parts of the application);
- Scalable fonts are implemented, so, subtitle size is adjustable now;
- Multilanguage support is implemented. Custom language strings are loaded from
  mc0:/SMS/SMS.lng file (which is just a text file);
- Skin format is changed slightly, so, it's possible to create skins of arbitrary
  size using updated version of "SMS Skin generator" application. Old skins are not
  supported anymore. Don't use a very high resulution (> 800x600) skins, otherwise
  results are not guaranteed. Note: new filename for skin is "mc0:/SMS/SMS.skn"
  (for consistency with other filenames like "SMS.lng" and "SMS.pal");
- Added functionality to select custom subtitles. This works as follows: pressing
  "circle" pad button while .avi file is selected activates a "mini-browser" with
  .sub/.srt files found in the same directory as .avi. If there're no .sub/.srt
  files found then playback begins instantly. Pressing "triangle" pad button while
  in "mini-browser" cancels selection, and starts playback with default subtitle
  selection algorithm (same name as .avi etc.). Pressing "cross" or "circle" button
  while in "mini-browser" selects subtitle and starts playback;
- .avi files with associated subtitles are displayed using different icon;
- added a possibility to launch user defined application upon exit. There're 3
  options: boot browser and 2 user defined (in SMS.lng) applications (default
  are "mc0:/BOOT/BOOT.ELF" and "mc0:/B?DATA-SYSTEM/BOOT.ELF). This feature is still
  experimental one, so, results are not guaranteed and the only supported media for
  launched programs is memory card and CD-ROM (not tested).
  (thanks to "Marcus R. Brown" for the loader code example); 
- some new configuration options are added (thanks to "bix64" for IPCONFIG.DAT
  "editor");
- some other "cosmetic" changes are made;
  Thanks to "Mr.Worm~~" for suggestions and testing;

Update (21-FEB-2006)
--------------------
- an attempt to fix "can't browse folders" bug;
  (thanks to everybody involved in this discovery);
- an attempt to fix "files on USB device are not selectable" bug;
- added possibility to decrease screen height in "display settings" ->
  "advanced settings" menu (thanks to "Voldemar_u2" for suggestion);

Update (22-FEB-2006)
--------------------
- fixed "Display subtitles" checkmark issue in player menu;
- fixed error reporting issue (too long messages leave onscreen artefacts);
- changed IOP reset routine (thanks to "dlanor" for the research/note);
- disabled internal queue locks by removing -DLOCK_QUEUES macro from the
  Makefile (it appears that queue locks are not needed since threads are
  non-preemptive anyway);

Update (23-FEB-2006)
--------------------
- changed loader code in attempt to fix "hangings" upon exit to
  user defined aplication;

Update (03-MAR-2006)
--------------------
- fixed incorrect subtitle position for multiline subtitles (thanks to
  "Voldemar_u2" for the note);
- sound made louder a bit (SPU2 volume values are adjusted);
- changed character indentation;
- added separate management for image offsets in different video modes
  (thanks to "Mr. Worm~~" and "Voldemar_u2" for the suggestion);
- added sound effects to the browser (for errors, usb/host mount/unmount
  and file/folder actions) - rather silly, just for demonstration of some
  SPU2 ADPCM decoding features;
- fixed a "crash" issue with some mp3 files (thanks to "sigmar" for the
  research/note);

Update (10-MAR-2006)
--------------------
- fixed "autoload subtitles" issue. Thanks to "suram884" for the remark;
- "Dolby Digital" passthrough mode (via S/PDIF optical output) is implemented
  (available through SMS Menu -> Player settings). Tested only with SoundBlaster
  PC sound card and 384kb/s ac3 streams. No volume adjustment is possible in
  this mode;
- fixed "Display Settings"/"TV system" menu issue (thanks to "Voldemar_u2" for
  the remark);
- fixed usb device hot swapping issue (bug in "usb_mass" device driver). Thanks
  to "hip203" for the research and note;

Update (16-MAR-2006)
--------------------
- Updated code for "Dolby Digital" output, so, any valid ac3 streams should be
  playable now (formerly only 5 channel streams were supported)(thanks to
  "lolo123" and "rathunter" for sample video clips);
- Added "play all mp3 files in the folder" functionality. It is activated by
  "circle" pad button while the folder is selected in the browser. Files are
  played in alphabethical order;
- USBD.IRX (free one, from ps2dev) is now integrated into SMS. No need for
  "external" usb driver anymore (thanks to "dlanor" for the suggestion);
- fixed "double speed ac3 sound" issue (thanks to "shiro" for the sample video
  clip);

Update (06-APR-2006)
--------------------
- Performance is improved (video decoder core is rewritten in assembler using
  R5900 CPU multimedia instructions). Mainly this affects video clips encoded
  with QPel/GMC options (these are kinda watchable now) though playback of
  "usual" video clips also got a bit smoother;
- Some motion compensation related bugs in video decoder are fixed and new ones
  are added :);

Update (10-APR-2006)
--------------------
- Fixed bug in video decoder (QPel motion compensation related). Thanks to "Crash"
  for sample video clip;
- Fixed scrollbar issue in the player (on-screen garbage in some cases);

Update (27-APR-2006)
--------------------
- Updated MP3 related code to deal with LAME-encoded files (thanks to
  "KanedA_PL" for the sample audio clip);
- Added "randomize playlist" and "repeat" options to the MP3 player.
  Note: "repeat" mode is applicable for both "m3u" and "mp3" files;
- Implemented rather primitive "audio spectrum display" feature during
  mp3 playback (8 bars for the left channel and 8 for the right one).
  It's not very precise for sample rates different from 48000Hz (since
  upsampling is performed on IOP side), but it just adds some fanciness
  to the "flying balls";
- Slightly improved performance of the MP3 decoder (assembler
  implementation of the 64-bit integer multiplication routine);

Update (10-MAY-2006)
--------------------
- Fixed "zero volume" bug in the player. Thanks to "wallacee" for the remark;
- An attempt has been made to get rid of "hangings" for "badly interleaved .avi"
  (thanks to "Mr.Worm~~" for sample video clip);
- Changed subtitle selectsion logic. Now pressing "triangle" button in the
  "minibrowser" cancels default subtitle selection ("dlanor's" suggestion);
- Audio spectrum is made "symmetric" ("rathunter's" suggestion);
- "Audio animation display" menu option is moved to "MP3 settings..." submenu;
- Timer code is rewritten;

Update (25-MAY-2006)
--------------------
- changed image management routines. Now SMS uses unified image format for
  skin and icons. IPU is fully utilized to perform image decompression (it's
  quite fast (a 640x480 test image is decompressed from 30KB to 900KB and
  uploaded to GS memory in just ~8ms)). A Win32 application (pic2smi.exe) is
  provided to create such an images. This application is replaced an old one
  (pic2sms.exe). It's possible to adjust image quality (better quality - bigger
  image size) and brightness, but default settings should work OK in most cases.
  These changes resulted smaller (by ~110KB uncompressed) SMS.elf size. Note new
  skin filename: SMS.smi ('i' means "image" :)) (thanks to "Mr. Worm~~" for
  trying it);
- high resolution movies are performing using reduced color resolution
  (dithered 16 bit), so no artefacts due to insufficient GS RAM should occur.
  There's also a possibility to force desired color resolution via SMS menu
  as 16 bit color resolution can improve performance a bit (less data traffic).
  (thanks to "Mr. Worm~~" again for trying);
- non-interleaved .avi's are treated as "unsupported". File treated as
  non-interleaved if it contains more than 256 consecutive packets belonging
  to the same stream (audio, video or other);
- added function to copy selected file to HDD's current partition. Pressing
  "square" pad button activates file context menu which currently has only
  one item (Copy to HDD). Copy process can be terminated by "triangle" (there
  can be some delay due to buffering, so press and hold "triangle" till "Stopping"
  message will appear at the status line). This function is purely experimental
  (just to measure device speed), so, use it on your own risk :) (files will be
  overwritten without any prompt). Note also that only PS2 internal HDD is
  supported;

Update (06-JUL-2006)
--------------------
- fixed "...the .srt subtitle file which is selected by entering "Select subtitles"
  menu can't be loaded with .avi if they are stored in mass:..." bug. Thanks to
  "magusneo" for the remark;
- added remote control support and removed gamepad support for slot 1:
  * for PS2 models without built-in IR sensor AND WITH rom1:RMMAN module an external
    IR sensor should be plugged into slot 1;
  * for PS2 models without built-in IR sensor AND WITHOUT rom1:RMMAN module SMS looks
    for mc0:/SMS/RMMAN.IRX module which is just a copy of rom1:RMMAN module from PS2
    models where this module is present. An external IR sensor should be plugged into
    slot 1;
  * for PS2 models with built-in IR sensor an external IR sensor has no effect;
  * for PS2 models without built-in IR sensor AND without rom1:RMMAN module AND
    without mc0:/SMS/RMMAN.IRX module remote control support is disabled;
  * Button mappings in SMS file browser:
    - "Reset"    - turns console off;
    - "Enter"    - equivalent to "cross" gamepad button;
    - "Play"     - equivalent to "cross" gamepad button;
    - "A-B"      - display about;
    - "Menu"     - display SMS menu;
    - "Top menu" - activate device browser menu (ignored in the device browser);
    - "Subtitle" - equivalent to "circle" gamepad button (ignored in the device browser);
    - "Program"  - equivalent to "square" gamepad button (ignored in the device browser);
    - "Return"   - equivalent to "triangle" gamepad button (ignored in the device browser);
  * Button mappings in SMS player:
    - "Pause"   - pause;
    - "Menu"    - player menu;
    - "Play"    - resume;
    - "Reset"   - equivalent to "triangle" gamepad button;
    - "Return"  - equivalent to "triangle" gamepad button;
    - "Stop"    - equivalent to "triangle" gamepad button;
    - "Scan >>" - equivalent to "right" gamepad button;
    - "Scan <<" - equivalent to "left" gamepad button;
    - "Display" - equivalent to "square" gamepad button;
    - "Time"    - equivalent to "cross" gamepad button;
    - "Audio"   - equivalent to "circle" gamepad button;

Note that in order to get remote control (with "external" IR sensor) working while
preserving gamepad and memory card functionalities unaffected a modified SIO2MAN
module is created (based on Marcus R. Brown code from SVN and my own disassembly
works) and integrated into SMS (source code of it can also be found in SVN).
THERE'S NO WARRANTY THAT OTHER DEVICES LIKE MULTITAP ETC. WILL WORK WITH IT.
Thanks to "Mr.Worm~~" and "Rathunter" for testing;

- added 'FMP4' fourcc recognition. Thanks to "ntt2005" for the research/note;
- added support for "simple" M3U list. Only relative paths are supported;
- updated some IOP drivers from ps2sdk;

Update (01-SEP-2006)
--------------------
- DTV 480p video mode support added. Note: not tested here;
- added menu option to configure controller port 2. There're 3 options:
  - gamepad: remote control is disabled and controller port 2 is configured
             for gamepad;
  - remote control: controller port 2 is configured for the official remote control
                    with external IR sensor plugged into controller slot 2;
  - none: controller port 2 is disabled and support for the official remote control
          with built-in IR sensor is enabled;
  Notes: second option will appear only if there's no rom1:RMMAN2 module;
- added brightness control to the player. Use 'L2' gamepad button to decrease
  brightness and 'R2' to increase it;
- added high precision mode to the MP3 decoder. Sound appears better in this mode but
  it's more demanding in computational sense. This mode is always on for audio files
  and it's selectable for video files via SMS menu;
- fixed cdvd driver (ISO9660:1999 mode). Thanks to 'Voldemar_U2' for the research and
  implementation;
- added support for arabic subtitles. Activated by
  "SMS menu->Player Settings->Subtitle Alignment->right" option. I'm not sure if I did
  it correctly (original strings are reversed and displayed right aligned), so any
  comments are welcome. Thanks to 'ALI' for explanations;
- updated USB drivers;
- network drivers are slightly optimized resulting a bit faster network transactions
  (I've got ~690KB/s while transferring a video file to the hard disk);

Update (20-SEP-2006)
--------------------
- fixed "image flicker" in DTV 480P video mode (thanks to "jacky401" for the
  note);
- hopefully fixed SMS crash if pause or menu is activated at the beginning of playback;
- 2 VESA video modes (640x480@60Hz and 640x480@75Hz) are added;
- changed logical screen width in DTV480p mode from 640 to 720 (I'm not sure if this is
  correct or not);
- this update uses VU0 unit to compute IDCT for intra macroblocks in micro mode, so,
  there's some performance gain in video decoding (~6%);
- there're also some experiments with thread priorities in the player, so there's no
  warranty that this1 thing will not hang during playback :);

Update (16-OCT-2006)
--------------------
- new MPEG audio decoder code. Fast (~40% faster than low precision one in the previous
  version), precise (uses floating point calculations) and supports MPEG 1.0 layers 1, 2
  and 3. For calculations VU0 (macro and micro modes) and VU1 (micro mode) are used;
  Note: layer 1 audio is not tested as I couldn't find any files encoded in this format.
  Thanks to 'mpg123' project creators/maintainers for the source code;
- 'ffgriever''s code (.txt subtitles) is integrated (thanks to him (her) for adding some
  code into this maze :));
- brighness control is updated a bit (if maximum is selected then a fixed value of 32 is
  added to each color component to make the whole image "brighter");
- some other internal changes (an attempt to eliminate "choppy playback" at the beginning
  of the movie playback (this can result longer pause at movie startup and during scrolling);

Update (20-OCT-2006)
--------------------
- fixed "bad sound from mp3 joint-stereo audio" bug
  (thanks to lee99 for the sample clip);
- fixed ".avi files with 'txts' chunk detected as mp3" bug
  (thanks to 'Z-Saber' for the sample clip);
- added possibility to playback .avi files with mp1/mp2 audio;
  (thanks to 'vitaliyfx' for the sample clip);
- added 'page-up/page-down' actions to the filebrowser (L1/L2 pad buttons);
- added '.mpa/.mp2' media filter to the filebroser;
- there's another guess about 480p mode;
- fixed 'SMS hangs when resumed from pause' bug;
  (thans to 'spkleader' for the note);
- fixed incorrect integration 'ffgriever''s code (.txt subtitles);

Update (24-OCT-2006)
--------------------
- fixed bugs related to mp3 sound glitches and m3u crashes;
  (thanks to 'lee99' for notes, patience and music);

Update (04-DEC-2006)
--------------------
- network driver is optimized further (host->hdd transfer rate is
  about 860 KB/s here and raw download speed is about 900KB/s);
- new data packet management -> the application might be unstable,
  since it was not tested a lot;
- sound driver is updated a bit;
- widescreen code of 'hjx' is integrated;
- added dynamic scrollbar scale (L1/R1 padd buttons during scroll);

Update (09-DEC-2006)
--------------------
- fixed "dynamic scrollbar scale" issue (thanks to 'peterdcrees' for
  the remark;
- speed up network driver once again a tiny bit (almost 900KB/s
  transfer speed to HDD here);
- changed A/V synchronization algorithm (image motions should appear
  smoother);

Update (24-DEC-2006)
--------------------
- added SMB/CIFS network protocol. Tested only on Windows XP Pro (SP2)
  and Linux/Debian with recent Samba distribution. Only one server
  connection is supported (though driver allows two). Network transfer
  speed is comparable to host: protocol (if the server supports raw data
  transfer (both XP and Linux do), otherwise it will be ~30% slower).
  To use this feature perform following steps:
  - create SMS.smb text file in a text editor. This file must contain
    exactly 5 lines in the following order:
    - IP address of the SMB server (ex. 192.168.0.3);
    - name of the SMB server       (ex. MyServer);
    - name of the PS2 (arbitrary)  (ex. MyPS2);
      Note: I'm not 100% sure about it, maybe this name
            must be listed in 'hosts'/'lmhosts' file;
    - user name used for authentication (ex. eugene);
    - user password used for authentication (ex. myPassword);
      Note 1: blank password is not allowed;
      Note 2: - "for individuals that think that harm is occurring, or
                 is going to occur, to him or her"
              - "for individuals that think the persecutor has the
                 intention to cause harm"
              PASSWORD IS ONE WAY ENCRYPTED BEFORE ITS TRANSMISSION TO
              THE SMB SERVER ONLY IF THE SERVER SUPPORTS SUCH A FEATURE
              (at least Windows XP Pro(SP2) and Samba on Libux/Debian do
              support this (please, don't consider this as advertisement));
  - make sure that IP address is correct;
  - make sure that SMB server name is correct;
  - make sure that SMB server allows connections (i.e. review firewall/router
    settings);
  - make sure that user whose credentials supplied in SMS.smb has appropriate
    access rights to connect to the SMB server;
  - copy this file to mc0:/SMS folder;
  - start SMS; If 'autostart network' option is active, then
    deactivate it, save settings and restart SMS;
  - goto SMS menu -> Device settings;
  - select SMB/CIFS network protocol;
  - start network support;
  New device icon shall appear as soon as connection to the server is
  established (or error message if there's a problem with authentication
  etc.). Normally this should occur in 0-10 seconds. There's no need to
  start SMB server before SMS, as connection is made automatically (SMS tries
  to establish connection to the server approximately each 5-10 seconds).
  It's possible to shut down SMB server while SMS browser is active. In this
  case SMB device icon will disappear and connection attempts are resumed again.
  This can however cause hang, so use this functionaliy at your own risk; 
  In case of problems use 'ethereal'/'wireshark' software to trace activity on
  the server's TCP port 139. New SMB device is browseable starting from share
  level (note that shares with '$' suffix in their name (like 'C$', 'ADMIN$'
  etc.) are explicitly disabled, so they won't ever appear in the file browser);
- increased packet buffer size in attempt to avoid audio.video 'stutter'
  (this results longer delays at startup and during scrolling);
- changed '-' and '_' character indentation to prevent overlapping
  (thanks to 'shassino' for the remark);
- added UTF8 subtitles support (thank to 'Npl' for the implementation);

Update (05-JAN-2007)
--------------------
- fixed crash issue with QPel-encoded clips. Thanks to 'darkangel84' for
  the sample clip;
- improved performance of the network driver (data transfer rate reached
  about 1.6MB/s here);

Update (11-02-2007)
--------------------
- fixed directory listing bug for DVD-Video disks. Thanks to 'phillyrider807'
  for the remarks;
- AC3 decoder is optimized a bit with "famous" R5900 assembler :) and a bug
  in (random crash) it fixed;
- an attempt to fix scrollbar issues;
- fixed network driver bug (random data corruption). Thanks to 'user112' for
  the report/research;
- updated USB driver;
- fixed incorrect subtitle line splitting. Thank to 'rainrix' for the note
  and sample subtitle file;
- first attempt to support MPEG1/2 video (program streams only). Not scrollable,
  so only 'pause'/'resume' actions are available (in future it can be implemented).
  Programmers can find it interesting as it fully uses PS2 hardware (IPU, MMI,
  scratchpad RAM with doublebuffering) for decoding. Interlaced MPEG2 video is
  supported (both frame/field encoding). CSS decryption is not supported. There might
  be bigger delays while loading individual mp3 files as program tries to detect
  file format automatically and mpeg1/2 and mp3 streams have no real headers to judge
  with 100% accuracy about the format. MPEG program streams without (or unsupported)
  video or/and audio might be incorrectly treated as mp3 (with a lot of noise), so
  don't damage your ears :). There's no "official" support of VCD as I don't have any
  (and no modchip here). Playback of .VOB files is not really supported either as
  they don't contain proper information about clip boundaries (I think that one comes
  from .IFO files). I've tested this stuff here only with some mpeg1 files downloaded
  via eMule and mpeg2 files recorded by Hauppauge WinTV PVR-350 PCI card on PC;
- updated SMB/CIFS driver in attempt to get it working with "cheap" NAS devices.
  Syntax of 'SMS.smb' file is relaxed, so username and password can be omitted.
  I've tested it here with Conceptronic CHD3LAN network HDD (DON'T EVER THINK TO BUY
  THIS ONE UNLESS YOU FEEL OK TO UPDATE ITS FIRMWARE as it shipped here with crappiest
  one I've ever seen (though after update (quite a process I must admit) it performs quite
  OK (to say nothing of the jet sound from its fan which I've just cut off (invalidating
  warranty thus) :D))). Note that password protected shares are not supported;
- presumably fixed a bug in GMC calculations;

Update (16-02-2007)
--------------------
- presumably fixed screwed MP3 code (random 'blips' while playing playlists)
  (thanks to 'lee99' and 'gukid' for the notes);

Update (23-02-2007)
--------------------
- implemented "scrolling" functionality for mpeg1/2 clips;
- fixed "freeze on video clip end" issue. Thanks to 'lee99' for the sample clip;
- fixed quite serious bug related to mpeg-4 code (crash in flames). Big thanks
  to 'user112' for the research and sample clip;

Update (18-03-2007)
--------------------
- fixed "short freeze" issue during back scrolling;
- fixed confusing GUI behavior during device hotplugging while in SMS menu;
- fixed corrupted video in some .avi files (QPel+AC3);
- presumably fixed freeze issue with .avi files while loading index;
- new VU IDCT microprogram (smaller and faster, so all VU microprograms are
  squeezed now in 4KB VU0 micro memory, leaving VU1 free for possible future
  usage (high quality colorspace conversion, for example :));
- fixed crash due to incorrectly formatted subtitles (error message
  is displayed instead) (Thanks to 'gogydm' for the sample file);
- fixed crash (in fact is "infinite loop") due to very long lines in
  subtitles (such lines are just truncated now). Thanks to 'rami1' for the
  research/note;

Update (23-03-2007) - SVN synchronization
-------------------
- fixed crash issue for .avi files with multiple AC3 audio streams
  (thanks to 'Tirabol' for the sample clip);
- fixed incorrect clip duration calculations for DivX6 files with
  subtitles (thanks to 'Tirabol' for the sample clip);
- changes due to upcoming implementation of DivX6 subtitles support;

Update (03-04-2007)
-------------------
- added support for subtitles embedded into .avi files produced by DivX6+
  sofrware. These subtitles are encoded as bitmaps, so no more problems
  with eastern etc. languages. Multiple language subtitles are supported
  and switchable via player menu during playback. Note that external
  subtitles (.srt etc.(if present/specified)) have precedence. Tested here
  with only one file provided by 'Tirabol' (thanks to him);
- added option to configure 'range level' (1-9) for AC3 audio streams (can be
  helpful to remove sound distortions). This option is also available in the
  player menu (thanks to 'orangpelupa' and 'eMaNoN' for the sample clips);
- added code to stop DVD drive at startup;
- added option (SMS menu -> Device settings) to disable CD/DVD autodetection;

Update (12-04-2007)
-------------------
- network and hdd drivers are updated. Note: there're quite significant changes
  in network divers code (DMA controller is employed to transfer data from the
  network adapter and data transfer speed and overall IOP performance are increased
  a bit) and I'm not 100% sure if this stuff is stable enough;
- fixed currupted sound issue for AC3 audio strams with 1 channel (mono)
  (thanks to 'Psychic Bison' and 'Bootlegninja' for the sample clip);
- fixed crash issue when player menu is activated during playback of .avi files
  without embedded subtitles while previously played clip contained them;
- slightly improved playback performance for .avi clips by using DMA stall control
  feature instead of interrupt handler;

Update (22-04-2007)
-------------------
- improved MPEG1/2 auto detection and playback for broken streams (thanks to  'topshooter'
  for the sample clip);
- network adapter driver is updated again;
- fixed bug in MPEG4 decoder (visible artifacts in some cases) (thanks to 'user112'
  for the sample clips);

Update (09-05-2007)
-------------------
- added menu option to modify display width (in 640-720 pixels range);
- network drivers are reverted back to non-DMA version as latest proved to be quite
  unreliable;
- added some more functionality to file management:
  1. copy directory tree to the "current" folder on internal HDD;
  2. delete directory tree from internal HDD;
  3. delete single file from internal HDD;
  4. copy single file to the "current" folder on internal HDD;
  This functionality is available via context menu ("square" gamepad button).
  "Current" means last visited folder before switching to another media.
  Note 1: point 1 is not implemented for 'host' and 'cddafs';
  Note 2: not fully tested. Appears to be working here. Use it on your own risk.
          'delete' operation is 'unix' style one, i.e. 'are you sure?' question
          is not asked (though operation can be aborted using 'triangle' on the
          gamepad (or 'stop' on the remote control));
- ogg/vorbis audio decoder (floating point implementation with PS2 optimized MDCT)
  is added. "mp3" and "ogg" files can be mixed in one "m3u" playlist. They can also
  be placed to the same folder and played in sequence using "circle" button while
  that folder is selected in the file browser;
  Note: "mono" ogg files and files with sampling rate other than 44100Hz were not
  tested as I don't have any;
- added menu option to control CD/DVD speed (low/medium/high);
- applied 'dave_t''s patch to handle remote control 'next'/'prev' buttons during
  playback. Thanks Dave ;);
- added code that stops rotation of the CD/DVD media during pause/scroll;
- added menu option to configure display color resolotion (32/16bits). Not really
  useful, just for fun :);

Update (20-05-2007)
-------------------
- changed brightness control algorithm;
- presumably fixed jerky .avi playback for DTV480P video mode (thanks to 'pitrz'
  for help, patience, sample clips and inspiration ;));
- added possibilty to change 'advanced display settings' for DTV480P/VESA modes;
- added support for .avi files with multiple audio tracks that have different
  audio formats (mp123/ac3). Thanks to 'cubi' for the sample clips;

Update (23-05-2007)
-------------------
- added menu option to configure pad directional buttons (either gamepad or
  remote control). Thanks to 'D1abo' for the code patch;
- lots of internal changes due to possible future implementation of 720p/1080i
  video modes and YUV -> RGB conversion using VU1. This could screw the things up,
  so, backup the previous version first as I don't have it anymore :). Bug reports
  are really appreciated. Points to pay attention on: DivX6 subtitles, overall font
  display and player OSD (volume/brightness control appearance etc.);

Update (08-06-2007)
-------------------
- fixed "crash" problem introduced with 'ogg' support for playlists;
- fixed jerky mpeg1/2 video due to incorrect memory alignment for audio buffer caused
  by some optimization tricks;
- added 720p and 1080i video modes. Only 16 bit dithered framebuffer is currently
  supported in these modes. Background image is not implemented in 1080i mode.
  Not tested here as I don't have appropriate TV. Thanks to 'pitrz' for the implementation.
- fixed issue that prevented .sub/.txt subtitles to be loaded if they contains empty
  lines like '{3557}{3629}'. Thanks to 'grange' for the notes;
- slightly changed remote control related code;

Update (17-06-2007)
-------------------
- hopefully fixed display issues for 720p and 1080i video modes (background image is supported in all modes now)
  (thanks to 'pitrz' for the implementation and 'D1abo' for the testing);
- fixed crash issue for 'ogg' files (thanks to 'vectis' for the sample clip);

Update (20-06-2007)
-------------------
- added functionality to launch applications from USB device ("mass:") upon exit;
- network adapter driver is rewritten. Data transfer rate is reached ~2+MB/s;
- new configuration options are added to the SMS menu:
 - 'Device settings -> Network settings' to configure network adapter operation mode.
   There're 3 options:
   - autonegotiation;
   - automatic (various modes are tried and first available is selected);
   - manual (duplex mode (half or full) and protocol (10Base-T or 100Base-TX));
 - 'Display settings -> Advanced settings' to fine tune video synchronization parameters
   (to avoid display flickering in different video modes). 'Parameter 1' is for 'audio
   playback' mode and 'Parameter 2' is for GUI/Browser mode. 
- fixed 'crash' issue for 'ogg' files (thanks to 'vectis' for the sample clips);

Update (01-SEP-2007)
--------------------
- added PS2 optimized DTS audio decoder. DTS passthrough mode (via S/PDIF optical output)
  is also supported (tested only with SoundBlaster PC sound card);
- added support for MPEG1/2 files with multiple audio tracks (selectable via player menu).
  Tested only with one .vob file that contains DTS and AC3 audio tracks;
- removed version number (now it is displayed only during initialization) from the GUI;
- improved MPEG1/2 display (now SMS takes into account widescreen aspect ratio (if present)
  from the original source for letterbox/pan-scan-1/2/3 display);
- added possibility to select background image (skin) in SMS menu (Browser settings ->
  Use background image option). Image file(s) should be in 'smi' format (i.e. produced by
  SMS image generator utility) and have '.smi' extension (case sensitive and without quotes).
  They should be placed inside 'mc0:/SMS/Skins' (case sensitive and without quotes) folder.
  No format verification is performed, so use only files produced by SMS image generator
  utility!!!
- changed display synchronization procedure for audio AND video playback. This can be somewhat
  model dependent, so if it won't work (black screen) then try to play with value for
  'Synchronization parameter 1' in 'SMS Menu' -> 'Display settings' -> 'Advance settings';
- fixed bug in SMS menu (thanks to 'elchevive' for the remark);
- network initialization is made asynchronous and nonblocking (i.e. it is performed in
  background even if the cable is not plugged in (it can be plugged in any time)). Maybe
  it is confusing a bit as 'Initializing network...' message is displayed for a very short
  time and further nothing seems to be happening;
- updated seek functionality for MPEG1/2 files;

Update (14-SEP-2007)
--------------------
- fixed screwed up audio spectrum display;
- added functionality to switch between SMB servers dynamically.
  SMS.smb file format is extended (it is still compatible with previous version)
  as follows:
  - first 5 lines remain the same as in the previous version (leave line
    4 and 5 blank (i.e. containing only newline character (no spaces)) if no username
    and password are used);
  - sixth line contains arbitrary text that will be displayed in SMS menu. The purpose
    of this line is to provide nice description for the SMB server defined in first
    5 lines;
  - aforementioned 6 lines can be further repeated to define more SMB servers;
  How it works: SMS Menu -> Device settings -> Network settings menu contains new
  item, namely 'SMB server' (note that this item will only appear if network protocol
  is set to 'SMB/CIFS'. Activation of this menu brings a submenu filled with servers listed
  in 'SMS.smb' file. There a new server connection can be selected. Note that if there is
  already connection in progress to the server that is down then breaking that connection
  can take a while (around 10 seconds here);
- updated audio driver and communication protocol with it. Now SMS uses "undocumented"
  SIF2 DMA channel to perform EE -> IOP data transfer. This will probably reduce chances
  of spontaneous crashes as there is no clash in SifRpc mechanism due to multithreading
  anymore. From the other side it can be model dependent, so it's possible that SMS won't
  work at all.
- user defined audio-video/subtitles-video offsets are made permanent and saved with
  SMS configuration. Thanks to 'psi88' for the suggestion;

Update (16-OCT-2007)
--------------------
- added forgotten brightness control for mpeg1/2 files. Thanks to 'elchevive' for the
  remark;
- improved compatibility with newer PS2 models;
- changed SMS configuration policy. 'SMS' folder can now be located on 'mc0' or 'mc1'
  depending on where SMS was launched from (if it was not launched from the memory card
  then 'mc0' is default). Note: do not remove memory card where SMS was started from
  after startup in order to save settings correctly;
- added functionality to update language, palette and SMB server list directly from SMS. It is
  accessible via file context menu ('square' pad button on file located on 'mass', 'cdfs' or
  'smb' device). Note: file name must be one of the following: 'SMS.lng', 'SMS.pal' or 'SMS.smb';
- added functionality to copy background images to 'SMS/Skins' folder directly from SMS.
  This is also accessible via file context menu (file extension must be '.smi'). Note that
  'Skins' subfolder is created automatically if it does not exists;

Update (13-NOV-2007)
--------------------
- improved performance of ogg/vorbis audio decoder;
- PS2 optimized windows media audio decoder is added;
- raw PCM (16-bit) audio decoder is added (for .avi and .mpeg/.mpg files);
- ASF (WMA only) container support is added. .wma files can also be included in .m3u playlists
  or "folder-played" just like .mp3/.ogg files;
- M3U playlist file parsing code is rewritten;
- improved MP3 file autodetection;

Update (23-NOV-2007)
--------------------
- attempt to fix various glitches with .m3u playlist stuff. Thanks to 'Thorsten Tietz' and
  'lee99' for the remarks and sample files;
- added functionality to play all video clips in the selected folder;
- added a configuration parameter to control mp3 file autodetection process;
- slightly speeded up network data transfer;
- some other internal changes that can screw the thins up :D;
- added 16 bit color resolution for mpeg1/2 files (can be useful to avoid image corruption
  in HD video modes);

Update (08-DEC-2007)
--------------------
- fixed quite nasty bug in the mpeg1/2 decoder (blocky artefacts in some cases). Thanks to
  'orangpelupa' for the note and sample clips;
- fixed incorrect inclusion of folders into playlist upon "Play all audio" action; Thanks
  to 'vsub' and 'dlanor' for the research/note;
- fixed stupid typo that prevented custom greek font to be loaded;
- added functionality to convert DOS character encodeing to Windows one. This allows correct
  display of localized filenames while using SMB networking. 4 default conversion tables are
  provided: CP866 <-> CP1251 (DOSCyrilic <-> WinCyrillic), CP737 <-> CP1273 (DOSGreek <-> WinGreek),
  CP850 <-> CP1252 (DOSLatin1 <-> WinLatin1) and CP852 <-> CP1250 (DOSLatin2 <-> WinLatin2).
  Custom conversion tables are supported in form of 'cyrillic.mtx', 'greek.mtx', 'latin2.mtx' and
  'latin1.mtx' files placed in mc<x>:/SMS folder. These can be generated by 'SMSCP.exe' Win32
  command line application which is also provided. It's usage is pretty simple. For example to
  generate 'cyrillic.mtx' file use following command: 'SMSCP 866 1252 cyrillic.mtx'. 866 means
  codepage ID for DOS and 1252 - codepage ID for Windows. If character charset is changed while
  browsing SMB share then manual directory rescan is required (leave and enter folder of interest
  while in SMS browser). Apparently this stuff also depends on 'Regional setings' of the SMB server;
- added '?' character replacement by appropriate region letter for 'mc0:/B?DATA-SYSTEM' string
  for user defined language file (if that string is present);
- implemented exception handler that is invoked if something goes terribly wrong with SMS.
  It displays some technical information about the exception that can help to fix the problem.
  There's a possibility either to reboot SMS (if it was launched from a memory card) or exit to
  the PS2 browser;
- fixed corrupted background image issue. Thanks to 'MrJiggles' for the semark and sample files;

Update (22-DEC-2007)
--------------------
- fixed crash issue for .avi files with PCM audio. Thanks to "MrJiggles" for the sample clip.
  (this is quite important fix as it affects all video/audio formats supported by SMS);
- added support of RTL subtitles. Thanks to 'lior e' for the implementation;
- added new menu option that controls exception handler activation. Default value is 'off'.
  It can also be forcibly turned off at startup by pressing 'start-select' buttons on the gamepad
  in slot 0 during startup (before SMS GUI screen appears);
- new .elf loader code. Thanks to 'ffgriever' for help and testing;
- attempt to fix problems with "Play all" functionality. Thanks to 'vsub' for the remarks;
- added 'Play all' funtionality for the current folder. This is activated by 'R1-circle' gamepad
  button combination or 'Display' button on the remote control. Only USB, SMB and HDD devices
  are tested;
- MPEG4 video decoder is redesigned a bit. Some decoding performance increase can be noted;

Update (11-JAN-2008)
--------------------
- fixed bug related to localized folder names (impossible to browse) in SMB driver (thanks to
  'Hikarudo' for the remark and sample;
- changed font drawing algorithm;
- slightly changed .avi parser in attempt to fix incorrect header data. Thanks to 'Startropic1'
  for the sample clip;
- added support for MBCS subtitles;
- added support for big (max. 4GB) OpenDML .avi files. Thanks to 'KnoWei' for the sample clip.
  Note: big files have a hudreds of thousands of indices. Each index entry consumes 16 bytes,
  so system can simply run out of memory (SMS exception handler will be activated) under certain
  conditions (big resolutoins, subtitles, multibyte fonts etc.);

Update (26-JAN-2008)
--------------------
- slightly improved audio driver (removed noticeable 'clicks' for monaural sound data);
- fixed incorrect subtitle handling for MBCS (thank to 'cybereye' for the note);
- added OSD that provides various information for video clips. Available via "cross" gamepad
  button during playback;
- slightly changed expansion bay driver (dev9) and its load policy in order to decrease
  fan noise when HDD and (or) network are not used;
- AAC (LC and Main profiles) audio decoder is added (based on FAAD2 library, optimized for PS2);
- M4A audio file format (produced by iTunes) support is added;
- 'audio' remote control button is mapped to 'change audio track' action;
- file browser selection is made "wrapable";
- changed USB driver load policy. 'USBD.IRX' placed into 'mc<x>:/SMS' folder has precedence over
  built-in one;

Update (05-FEB-2008)
--------------------
- updated USB mass storage driver in attempt to raise data transfer speed. For my USB flash memory
  stick there's improvement from ~500KB/s to ~700KB/s. This is quite experimental stuff, so if
  this won't work then there's a possibility to load "old" version from 'mc<x>:/SMS' folder same
  way as for 'USBD.IRX' described above. Driver name must be 'USBM.IRX', however :). Note, that
  keeping filesystem on the USB device defragmented also improves performance !!!
- added support for raw .aac files (ADTS only). Thanks to 'hip203' for the sample files;
- added support for AAC audio in .avi files (codec ID 0x000000FF). Thanks to 'hip203' and 'MrJiggles'
  for the sample clips;
- updated SMS image generator. Added possibility to embed custom palette into generated image, so
  SMS palette can be managed by background image;

Update (01-MAR-2008)
--------------------
- Fixed problem with (S)VCD .DAT files. Thanks to 'Salacho' for the remark;
- USB mass storage driver is rewritten from scratch (though some elements from 'ps2dev's' one were used).
  Added support for up to 4 USB mass storage devices (FAT formatted) that are simultaneously connected
  to the console. USB devices with multiple logical units (like <x>-in-1 cardreaders) should also work
  (tested here with SWEEX CF004 53-in-1 cardreader with CF and SD cards)). Transfer speed varies
  depending on filesystem fragmentation (peak rate exceeds 800KB/s sometimes (btw., maximum raw transfer
  rate (without filesystem overhead) I've got here is around 950 KB/s which is the same as on my PC
  (USB 1.1 slot). If this driver will cause problems then there's still a possibility to use older driver
  as described in changelog from 05-FEB-2008;
- Improved .aac file autodetection algorithm. Thanks to 'lee99' for the sample clip;
- Added support for HE-AAC sound. Thanks to everybody who provided sample clips;

Update (01-APR-2008)
--------------------
- added FLAC audio support;
- added small (32 entries FIFO) history buffer for video clips. It is saved into 'SMS' folder on the memory
  card. History is based on the absolute file name. Only clips that were ended by the user action (triangle
  gamepad button) are added/updated to the history buffer and the buffer is saved automatically, so there
  can be some pause when playback is ended by the user;
- added support for raw .ac3 audio files;
- added functionality to control vertical image position for video clips. It is available via player menu
  during playback and may be useful to display subtitles more nicely;
- updated USB mass storage driver (added device handling with sector sizes other than 512 bytes);
- updated network driver;

Update (12-APR-2008)
--------------------
- fixed small problem (repeat glitch at the end) related to FLAC decoder (thanks to 'vectis' for the
  remark/sample clip);
- added suport for FLAC files containing ID3 tags (these are currently ignored) (thanks to 'vectis' for
  the remark/sample clip);
- updated USB mass storage driver;

Update (29-APR-2008)
--------------------
- fixed problem with .avi embedded subtitles. Thanks to 'KingFire' for the sample clip;
- fixed bug related to SMB connectivity (probably this also fixes some other issues (freezing
  at startup etc.)). Thanks to 'pastito' for the research and help;

Update (17-MAY-2008)
--------------------
- updated .avi embedded subtitles handling. Thanks to 'KingFire' for sample clip, testing and patience.

Update (01-JUL-2008)
--------------------
- updated OSD information panel (added available memory indication at IOP side);
- video decoders are redesigned in order to reduce RAM usage;
- implemented 32 bit color resolution for 720p (playback and browser modes) and
  1080i (playback mode only) video modes. Note: synchronization parameter 1 and display
  position adjustments might be necessary for these modes in order to avoid some
  screen artefacts. Note: synchronization parameter 3 was added. This one affects audio
  playback, so in total there're 3 parameters related to video synchronization:
    - parameter 1 is for video clips;
    - parameter 2 is for GUI;
    - parameter 3 is for audio playback;
  These parameters are quite important, so if there're some unwanted screen effects
  (broken picture during scene motion), screen flickering etc. then adjusting these parameters
  can remove aforementioned effects. Each video mode has its own independent parameter set.
  For parameter 1 SMS can approximate its value automatically by choosing 'auto' (formerly '0')
  value;
- redesigned subtitle management code;
- improved MPEG1/2 scrolling/positioning;
- added 2 "eye candies" (clock and "please wait" indicator);
- plan "B" loader/installer is published;

Update (05-JUL-2008)
--------------------
- bugfixes;

Update (02-SEP-2008)
--------------------
- some optimizations regarding avi indices;
- fixed some bugs in the FAT (USB) filesystem driver;
- fixed issues with .sub/.txt subtitles. Thanks to 'Karol Nowak'
  for the research and sample files;

Update (14-NOV-2008)
--------------------
- fixed incorect subtitle alignment when subtitle font horizontal size
  is not equal to the default value of 32;
- slightly changed file autodetection algorithm;
- added baseline JPEG decoder (written from scratch) and rather primitive
  JPEG viewer. Sequence of images can also be viewed using the same
  button combinations as for audio/video playlists ("circle" for folder and
  "R1-circle" for the current folder). While in viewer following gamepad and
  remote control buttons can be used:
  - left       : previous slide;
  - right/cross: next slide;
  - triangle   : exit;
  - rectangle  : zoom-in/zoom-out (only for images with resolution lower
                 than screen one);
  Note: decoder supports only baseline JPEGs at the moment. Progressive,
        loseless and other "exotic" files are not currently supported.
        Maximum image resolution that has been tested is 6912x4608;

Update (07-DEC-2008)
--------------------
- added "rotate image" function for JPEG viewer:
  - "circle"/"angle" - rotate image counterclockwise;
  - "R1"/"shuffle"   - rotate image clockwise;
- fixed bug in MPEG4 decoder;

Update (18-JAN-2009)
--------------------
- refactored I/O code (got rid of iomanX/fileXio stuff). This is actually
  a first step to the new I/O subsystem that I am planning to implement;
- fixed glitch in 1080i video mode;
- fixed power off problem;
- chnaged A/V synchronization code yet again in order to prevent application
  lock-up for badly interleaved .avi files;
- added functionality to configure "reset" button action (power-off/exit) in
  "SMS menu -> Browser settings";

Update (01-APR-2009)
--------------------
- updated USB/FAT driver. Thanks to Matzino for help in testing;
- improved M3U playlist randomization;

Update (11-SEP-2009)
--------------------
- implemented 576p video mode (thanks to HRGS for the implementation);
- implemented 2:2 video pulldown (for smooth playback of 24fps video clips in
  PAL/576p video modes) (thanks to HRGS for the implementation);
- updated audio driver (modified upsampling algorithm that provides better
  sound quality and more flexible handling)(thanks to HRGS for the implementation);
- changed stereo downmixing algorithm for AC3 audio (using Dolby ProLogic II matrix);
  (thanks to HRGS for the research/implementation);
- significally changed USB/FAT driver (speed improvements etc.);
```
