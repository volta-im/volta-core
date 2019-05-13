// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2018-2018 The Volta Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VOLTA_QT_VoltaADDRESSVALIDATOR_H
#define VOLTA_QT_VoltaADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class VoltaAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VoltaAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Volta address widget validator, checks for a valid volta address.
 */
class VoltaAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VoltaAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // VOLTA_QT_VoltaADDRESSVALIDATOR_H
