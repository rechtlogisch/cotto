#ifndef OTTO_STATUSCODE_H_
#define OTTO_STATUSCODE_H_

/**
 * @file
 * @brief Auflistung der Otto-Statuscodes
 */
typedef enum {
    /** Die Verarbeitung ist ordnungsgemäß abgeschlossen worden. */
    OTTO_OK = 0,


    // ========   Otto-Fehler 61 040 x xxx   ========


    // --------   Interne Fehler 61 040 1 xxx   --------

    /** Es trat ein interner Fehler auf, Details stehen im otto.log. */
    OTTO_INTERNER_FEHLER = 610401001,

    /** Es trat ein unerwarteter Fehler auf, Details stehen im otto.log. */
    OTTO_UNBEKANNTER_FEHLER,

    /** Es trat ein Fehler bei der Erzeugung eines Ad-hoc-Zertifikats für den nPA auf, Details stehen ggf. im otto.log. */
    OTTO_NPA_ZERTIFIKATFEHLER,

    // --------   Transferfehler 61 040 3 xxx   --------

    /** Es trat ein Fehler beim Transfer auf, Details stehen ggf. im otto.log. */
    OTTO_TRANSFER_FEHLER = 610403001,

    /** Es trat ein Fehler bei der Initialisierung des Transfers auf. */
    OTTO_TRANSFER_INIT,

    /** Es konnte keine Verbindung zu den Otter-Servern aufgebaut werden. */
    OTTO_TRANSFER_CONNECTSERVER,

    /** Es konnte keine Verbindung zum Proxy aufgebaut werden. */
    OTTO_TRANSFER_CONNECTPROXY,

    /** Bei der Kommunikation mit dem Server kam es zu einer Zeitüberschreitung. */
    OTTO_TRANSFER_TIMEOUT,

    /** Der Proxy erwartet Anmeldedaten oder der Proxy hat die Verbindung abgelehnt. */
    OTTO_TRANSFER_PROXYAUTH,

    /** Der Client darf die Schnittstelle nicht verwenden. */
    OTTO_TRANSFER_UNAUTHORIZED,

    /** Der Otter-Server hat das Objekt nicht gefunden. */
    OTTO_TRANSFER_NOT_FOUND,

    /** Der Otter-Server hat einen unerwarteten Fehler gemeldet. Möglicherweise ist ein Retry sinnvoll. Details stehen im otto.log. */
    OTTO_TRANSFER_SERVER_FEHLER,

    /** Die empfangenen Daten konnten nicht dekodiert werden. */
    OTTO_TRANSFER_DECODING,

    /** Es konnte kein Ad-hoc-Zertifikat für den Personalausweis oder den Aufenthaltstitel erzeugt bzw. gefunden werden, Details stehen ggf. im otto.log. */
    OTTO_TRANSFER_EID_ZERTIFIKATFEHLER,

    /** Der eID-Client ist nicht erreichbar. Wahrscheinlich wurde er nicht gestartet oder die übergebene lokale URL ist nicht korrekt. */
    OTTO_TRANSFER_EID_KEINCLIENT,

    /** Für die Identifikationsnummer des Benutzers existiert kein Konto bei ELSTER. */
    OTTO_TRANSFER_EID_KEINKONTO,

    /** Der eID-Client hat einen Fehler gemeldet. Details zu dem Fehler finden Sie im Log des eID-Clients oder ggf. im otto.log. */
    OTTO_TRANSFER_EID_CLIENTFEHLER,

    /** Der Personalausweis wird von einem anderen Vorgang blockiert. Beenden Sie den anderen Vorgang und versuchen Sie es dann erneut. */
    OTTO_TRANSFER_EID_NPABLOCKIERT,

    // --------   Anwendungsfehler 61 040 5 xxx   --------

    /** Einer der übergebenen Parameter ist ungültig. */
    OTTO_UNGUELTIGER_PARAMETER = 610405001,

    /** Das übergebene Handle ist ungültig. */
    OTTO_UNGUELTIGES_HANDLE,

    /** Die übergebene Otto-Instanz wird gerade (zum Beispiel in einem anderen Thread) verwendet. */
    OTTO_MEHRFACHAUFRUFE_NICHT_UNTERSTUETZT,

    /** Eines der übergebenen Otto-Objekte wurde mit einer anderen Otto-Instanz erstellt. */
    OTTO_INSTANZEN_INKONSISTENT,

    /** Mit dieser Instanz wurden Unterobjekte erzeugt, die noch nicht freigegeben worden sind. */
    OTTO_INSTANZ_UNTEROBJEKTE_NICHT_FREIGEGEBEN,

    /** Die Protokolldatei konnte nicht erzeugt oder geöffnet werden. */
    OTTO_LOG_FEHLER,

    /** Die verwendetet Funktion oder Funktionalität wird nicht, noch nicht oder nicht mehr unterstützt. */
    OTTO_FUNKTION_NICHT_UNTERSTUETZT,

    /** Für das Zertifikat wurde ein falsches Passwort bzw. eine falsche PIN angegeben. */
    OTTO_ZERTIFIKAT_PIN_FALSCH,

    /** Unter dem angegebenen Pfad wurde kein Zertifikat gefunden. */
    OTTO_ZERTIFIKAT_PFAD_FALSCH,

    /** Das Zertifikat wurde nicht erkannt, Details stehen ggf. im otto.log. */
    OTTO_ZERTIFIKAT_NICHT_ERKANNT,

    /** Die Prüfsumme wurde bereits finalisiert */
    OTTO_PRUEFSUMME_FINALISIERT,

    /** Es wurde keine oder eine ungültige Hersteller-ID in den Einstellungen gesetzt. */
    OTTO_UNGUELTIGE_HERSTELLERID,

    /** Der Empfang wurde durch einen API-Aufruf vorzeitig beendet. */
    OTTO_EMPFANG_VORZEITIG_BEENDET,

    /** Die Versanddaten dürfen die Mindestgröße nicht unterschreiten. */
    OTTO_VERSAND_GERINGE_DATENMENGE,

    /** Die eSigner-Bibliothek konnte nicht geladen werden, Details stehen ggf. im otto.log */
    OTTO_ESIGNER_NICHT_GELADEN,

    /** Die eSigner-Bibliothek ist veraltet, Details stehen ggf. im otto.log. */
    OTTO_ESIGNER_VERALTET,

    /** Die eSigner-Bibliothek ist mit der Otto-Bibliothek inkompatibel. */
    OTTO_ESIGNER_INKOMPATIBEL,

    /** Es wurde keine URL oder IP für den Proxy angegeben. */
    OTTO_PROXY_URL,

    /** Es wurde kein oder ein ungültiger Port für den Proxy angegeben. */
    OTTO_PROXY_PORT,

    /** Es wurde kein gültiges Proxy-Authentifizierungschema angegeben. */
    OTTO_PROXY_AUTHSCHEMA,


    // --------   Anwendungsfehler, die vom eSigner durchgereicht werden 61 040 5 8xx   --------

    /** eSigner: Überlastung */
    OTTO_ESIGNER_BUSY = 610405801,

    /** eSigner: Fehler beim Entschlüsseln */
    OTTO_ESIGNER_DECRYPT,

    /** eSigner: Fehler beim Verschlüsseln */
    OTTO_ESIGNER_ENCRYPT,

    /** eSigner: Fehler beim Encoding */
    OTTO_ESIGNER_ENCODE_ERROR,

    /** eSigner: Parameter Fehler: unbekanntes Encoding */
    OTTO_ESIGNER_ENCODE_UNKNOWN,

    /** eSigner: Eine Laufzeitausnahme ist aufgetreten und abgefangen worden. */
    OTTO_ESIGNER_ESICL_EXCEPTION,

    /** eSigner: Ungültiges Token-Handle */
    OTTO_ESIGNER_INVALID_HANDLE,

    /** eSigner: PKCS11- bzw. PC/SC-Bibliothek fehlt oder ist nicht ausführbar. */
    OTTO_ESIGNER_LOAD_DLL,

    /** eSigner: Zu viele Sessions geöffnet. */
    OTTO_ESIGNER_MAX_SESSION,

    /** eSigner: Der PC/SC-Dienst ist nicht gestartet. */
    OTTO_ESIGNER_NO_SERVICE,

    /** eSigner: Kein Signatur/Verschlüsselungszertifikat bzw. -schlüssel vorhanden. */
    OTTO_ESIGNER_NO_SIG_ENC_KEY,

    /** eSigner: Speicherallokation fehlgeschlagen. */
    OTTO_ESIGNER_OUT_OF_MEM,

    /** eSigner: Fehler beim Zugriff auf Hard-Token-Entschlüsselungsschlüssel */
    OTTO_ESIGNER_P11_ENC_KEY,

    /** eSigner: Die PKCS#11-Engine wird von einer anderen Bibliothek belegt. */
    OTTO_ESIGNER_P11_ENGINE_LOADED,

    /** eSigner: P11 Initialer Token-Zugriff fehlgeschlagen. */
    OTTO_ESIGNER_P11_INIT_FAILED,

    /** eSigner: P11 Verschlüsselungszertifikat fehlt. */
    OTTO_ESIGNER_P11_NO_ENC_CERT,

    /** eSigner: P11 Signaturzertifikat fehlt. */
    OTTO_ESIGNER_P11_NO_SIG_CERT,
    /** eSigner: Fehler beim Zugriff auf den Hard-Token-Signaturschlüssel */
    OTTO_ESIGNER_P11_SIG_KEY,
    /** eSigner: Leere Slot-Liste, d.h. keine Karte eingesteckt. */
    OTTO_ESIGNER_P11_SLOT_EMPTY,

    /** eSigner: Temporäres PKCS#12-Token konnte nicht erzeugt werden. */
    OTTO_ESIGNER_P12_CREATE,

    /** eSigner: Fehler beim Dekodieren des PKCS#12-Objekts */
    OTTO_ESIGNER_P12_DECODE,

    /** eSigner: Fehler beim Zugriff auf den Soft-PSE-Entschlüsselungsschlüssel */
    OTTO_ESIGNER_P12_ENC_KEY,

    /** eSigner: Fehler beim Zugriff auf den Soft-PSE-Signaturschlüssel */
    OTTO_ESIGNER_P12_SIG_KEY,

    /** eSigner: P12 Verschlüsselungszertifikat fehlt. */
    OTTO_ESIGNER_P12_NO_ENC_CERT,

    /** eSigner: P12 Signaturzertifikat fehlt. */
    OTTO_ESIGNER_P12_NO_SIG_CERT,

    /** eSigner: Fehler beim Lesen des PKCS#12-Objekts */
    OTTO_ESIGNER_P12_READ,

    /** eSigner: Fehler beim Dekodieren des PKCS#7-Objekts */
    OTTO_ESIGNER_P7_DECODE,

    /** eSigner: Fehler beim Lesen des PKCS#7-Objekts */
    OTTO_ESIGNER_P7_READ,

    /** eSigner: Das Entschlüsselungszertifikat ist nicht in der Empfängerliste enthalten. */
    OTTO_ESIGNER_P7_RECIPIENT,

    /** eSigner: Die PIN bzw. das Passwort ist gesperrt. */
    OTTO_ESIGNER_PIN_LOCKED,

    /** eSigner: Die PIN bzw. das Passwort ist falsch. */
    OTTO_ESIGNER_PIN_WRONG,

    /** eSigner: Der Pfad zum Soft-PSE ist falsch oder ungültig. */
    OTTO_ESIGNER_PSE_PATH,

    /** eSigner: Fehler beim Zugriff auf den Stick-Entschlüsselungsschlüssel. */
    OTTO_ESIGNER_SC_ENC_KEY,

    /** eSigner: Initialer Token-Zugriff auf die PC/SC-Schnittstelle fehlgeschlagen. */
    OTTO_ESIGNER_SC_INIT_FAILED,

    /** eSigner: Kein unterstütztes Applet gefunden. */
    OTTO_ESIGNER_SC_NO_APPLET,

    /** eSigner: PC/SC Verschlüsselungszertifikat fehlt. */
    OTTO_ESIGNER_SC_NO_ENC_CERT,

    /** eSigner: PC/SC Signaturzertifikat fehlt. */
    OTTO_ESIGNER_SC_NO_SIG_CERT,

    /** eSigner: Fehler in der Karten-Session. */
    OTTO_ESIGNER_SC_SESSION,

    /** eSigner: Fehler beim Zugriff auf den Stick-Signaturschlüssel */
    OTTO_ESIGNER_SC_SIG_KEY,

    /** eSigner: Es ist keine Karte bzw. kein Stick eingesteckt. */
    OTTO_ESIGNER_SC_SLOT_EMPTY,

    /** eSigner: Der Token-Typ der CA (Certification Authority, Zertifizierungsstelle) stimmt nicht mit dem internen Token-Typ überein. */
    OTTO_ESIGNER_TOKEN_TYPE_MISMATCH,

    /** eSigner: Die Aktion wurde vom Benutzer abgebrochen. */
    OTTO_ESIGNER_USER_CANCEL,

    /** eSigner: Die Zertifikatskette konnte nicht verifiziert werden. */
    OTTO_ESIGNER_VERIFY_CERT_CHAIN,

    /** eSigner: Die Datenstruktur ist nicht initialisiert. */
    OTTO_ESIGNER_DATA_NOT_INITIALIZED,

    /** eSigner: Der Lesepuffer zum Dekodieren der ASN.1-Struktur ist zu klein. */
    OTTO_ESIGNER_ASN1_READ_BUFFER_TOO_SMALL,

    /** eSigner: Die Daten der ASN.1-Struktur sind unvollstaendig. */
    OTTO_ESIGNER_ASN1_READ_DATA_INCOMPLETE,

    // --------   Systemfehler 61 040 7 xxx   --------

    /** Fehler beim Einlesen der otto.ini-default. */
    OTTO_INIDATEI_LESEFEHLER = 610407001,

    /** Das Zertifikat konnte nicht geladen werden. */
    OTTO_ZERTIFIKAT_LESEFEHLER,

    /** Die Zertifikatsdatei oder das Soft-PSE ist defekt. */
    OTTO_ZERTIFIKAT_DEFEKT,

    /** Für das Zertifikat konnte kein Fingerabdruck erstellt werden. */
    OTTO_ZERTIFIKAT_FINGERABDRUCK_FEHLER,

    /** Die Daten konnten mit dem übergebenen Zertifikat nicht signiert werden. */
    OTTO_SIGNIEREN_FEHLGESCHLAGEN,

    /** Die Daten konnten mit dem übergebenen Zertifikat nicht entschlüsselt werden. */
    OTTO_ENTSCHLUESSELN_FEHLGESCHLAGEN,

    /** Die Daten konnten nicht dekomprimiert werden. */
    OTTO_DEKOMPRESSION_FEHLGESCHLAGEN,

    /** Es ist nicht genügend Arbeitsspeicher vorhanden. */
    OTTO_NICHT_GENUEGEND_ARBEITSSPEICHER
} OttoStatusCode;

#endif /* OTTO_STATUSCODE_H_ */
