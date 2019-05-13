
Debian
====================
This directory contains files used to package voltad/volta-qt
for Debian-based Linux systems. If you compile voltad/volta-qt yourself, there are some useful files here.

## volta: URI support ##


volta-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install volta-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your volta-qt binary to `/usr/bin`
and the `../../share/pixmaps/volta128.png` to `/usr/share/pixmaps`

volta-qt.protocol (KDE)

