#!/bin/bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

VoltaD=${VoltaD:-$BINDIR/voltad}
VoltaCLI=${VoltaCLI:-$BINDIR/volta-cli}
VoltaTX=${VoltaTX:-$BINDIR/volta-tx}
VoltaQT=${VoltaQT:-$BINDIR/qt/volta-qt}

[ ! -x $VoltaD ] && echo "$voltad not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
XVTVER=($($VoltaCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for voltad if --version-string is not set,
# but has different outcomes for volta-qt and volta-cli.
echo "[COPYRIGHT]" > footer.h2m
$voltad --version | sed -n '1!p' >> footer.h2m

for cmd in $voltad $VoltaCLI $VoltaTX $VoltaQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${XVTVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${XVTVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
