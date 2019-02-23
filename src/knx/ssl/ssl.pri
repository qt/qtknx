HEADERS += ssl/qknxcryptographicengine.h \
           ssl/qknxsecurekey.h \
           ssl/qknxsecureconfiguration.h \
           ssl/qknxsecureconfiguration_p.h \
           ssl/qknxssl_p.h

SOURCES += ssl/qknxcryptographicengine.cpp \
           ssl/qknxsecurekey.cpp \
           ssl/qknxsecureconfiguration.cpp \
           ssl/qknxssl_openssl.cpp

qtConfig(opensslv11) { # OpenSSL 1.1 support is required.
    SOURCES += ssl/qsslsocket_openssl_symbols.cpp
    HEADERS += ssl/qsslsocket_openssl_symbols_p.h \
               ssl/qsslsocket_openssl11_symbols_p.h

    QMAKE_CXXFLAGS += -DOPENSSL_API_COMPAT=0x10100000L

    # Add optional SSL libs
    # Static linking of OpenSSL with msvc:
    #   - Binaries http://slproweb.com/products/Win32OpenSSL.html
    #   - also needs -lUser32 -lAdvapi32 -lGdi32 -lCrypt32
    #   - libs in <OPENSSL_DIR>\lib\VC\static
    #   - configure: -openssl -openssl-linked -I <OPENSSL_DIR>\include -L <OPENSSL_DIR>\lib\VC\static OPENSSL_LIBS="-lUser32 -lAdvapi32 -lGdi32" OPENSSL_LIBS_DEBUG="-lssleay32MDd -llibeay32MDd" OPENSSL_LIBS_RELEASE="-lssleay32MD -llibeay32MD"

    qtConfig(openssl-linked): \
        QMAKE_USE_FOR_PRIVATE += openssl
    else: \
        QMAKE_USE_FOR_PRIVATE += openssl/nolink
    win32: LIBS_PRIVATE += -lcrypt32
}
