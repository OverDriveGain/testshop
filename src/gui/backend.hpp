#pragma once

#ifndef BACKEND_HPP_NEROSHOP
#define BACKEND_HPP_NEROSHOP

#include <QObject>
#include <QUrl>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>

#include "../core/converter.hpp"
#include "../core/validator.hpp"
#include "../core/seller.hpp"

#include "wallet_proxy.hpp"

#include <iostream>

namespace neroshop {
class Backend : public QObject { // This class was created for storing utility functions and backend implementations // Maybe I should rename this to BackendTools?
    Q_OBJECT 
public:
    Q_INVOKABLE QString urlToLocalFile(const QUrl& url) const;
    Q_INVOKABLE void copyTextToClipboard(const QString& text);
    
    Q_INVOKABLE double convertXmr(double amount, const QString& currency, bool to) const;
    Q_INVOKABLE QString getCurrencySymbol(const QString& currency) const;
    Q_INVOKABLE bool isSupportedCurrency(const QString& currency) const;
    
    //Q_INVOKABLE bool initializeDatabase() const;
    Q_INVOKABLE void testFunction(gui::Wallet * wallet);
    
    Q_INVOKABLE void registerUser();
    Q_INVOKABLE void loginWithWalletFile();
    Q_INVOKABLE void loginWithMnemonic();
    Q_INVOKABLE void loginWithKeys();
    Q_INVOKABLE void loginWithHW();
    
    // catalog functions here
    // seller/user functions here
private:
};
}
#endif
