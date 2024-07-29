#ifndef OTTO_H_
#define OTTO_H_

#include "otto_statuscode.h"
#include "otto_types.h"

#include <stdint.h>

/**
 * @file
* @brief Deklaration der Otto-Funktionen
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Erstellt und initialisiert eine neue Otto-Instanz.
 *
 * Das Erzeugen einer Otto-Instanz ist ressourcen- und zeitintensiv.
 * Daher sollten nicht mehr Instanzen erzeugt werden, als gleichzeitig
 * benutzt werden sollen.
 *
 * Speziell braucht nicht für jeden neuen Thread eine neue Instanz erstellt
 * zu werden, da die Otto-Instanzen nicht an ihre Ersteller-Threads
 * gebunden sind. Otto-Instanzen dürfen _nicht gleichzeitig_ in mehreren
 * Threads verwendet werden, aber sie dürfen wechselnd von verschiedenen
 * Threads verwendet werden. Das heißt insbesondere, dass sie von neuen
 * Threads wiederverwendet werden dürfen.
 *
 * Otto-Instanzen sind in dem Sinne threadsicher, dass verschiedene
 * Otto-Instanzen zeitgleich in verschiedenen Threads verwendet werden können.
 * Jedoch darf ein- und dieselbe Otto-Instanz nicht zeitgleich in mehreren
 * Threads verwendet werden.
 *
 * @param[in]  logPfad
 *             Optionaler Pfad zur Log-Datei otto.log. Ist der
 *             Wert gleich @c NULL, wird das betriebssystemspezifische
 *             Verzeichnis für temporäre Dateien verwendet.
 *
 * @param[in]  logCallback
 *             Callback-Funktion, die gegebenenfalls von Otto bei der
 *             Protokollierung von Meldungen aufgerufen wird.
 *             Siehe ::OttoLogCallback
 *             Der Parameter darf @c NULL sein.
 *
 * @param[in]  logCallbackBenutzerdaten
 *             Beliebiger Zeiger auf Daten, den Otto beim Aufruf eines
 *             @p logCallback an den Callback weiterreicht.
 *             Über diesen Weg kann sich eine Anwendung eigene Daten an
 *             ihre Log-Callback-Funktion übergeben lassen.
 *             Der Parameter darf @c NULL sein.
 *
 * @param[out] instanz
 *             Handle der erzeugten Otto-Instanz
 *
 * @note       Kann kein otto.log angelegt werden, wird eine entsprechende
 *             Fehlermeldung auf die Konsole (stderr) geschrieben und an den
 *             Windows-Ereignisdienst bzw. den syslogd-Dienst (Linux, AIX, macOS)
 *             geschickt.
 *             Für Linux, AIX und macOS ist zu beachten, dass der syslogd-Dienst
 *             gegebenenfalls erst noch zu aktivieren und für die Protokollierung
 *             von Meldungen der Facility "User" zu konfigurieren ist.
 *             Suchkriterien für Otto-Meldungen in der Windows-Ereignisansicht
 *             sind "ERiC (Elster Rich Client)" als Quelle und "Anwendung"
 *             als Protokoll.
 *             Suchkriterien für ERiC-Meldungen in den Systemlogdateien
 *             unter Linux, AIX und macOS sind die Facility "User" und
 *             der Ident "ERiC (Elster Rich Client)".
 *
 * @see
 *             - OttoInstanzFreigeben()
 *             - OttoLogCallback
 */
OttoStatusCode OttoInstanzErzeugen(const byteChar *logPfad,
                                   OttoLogCallback logCallback,
                                   void *logCallbackBenutzerdaten,
                                   OttoInstanzHandle *instanz);

/**
 * @brief      Gibt eine Otto-Instanz frei.
 *
 * Die freigegebene Otto-Instanz sowie alle eventuell noch daran
 * gebundenen Objekte dürfen nach der Freigabe nicht mehr verwendet
 * werden.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, die freigegeben werden soll.
 *
 * @see
 *             - OttoInstanzErzeugen()
 */
OttoStatusCode OttoInstanzFreigeben(OttoInstanzHandle instanz);

/**
 * @brief      Erstellt ein Otto-Zertifikatsobjekt für ein Sicherheitstoken.
 *
 * Das Zertifikatsobjekt ist an die Otto-Instanz gebunden, für die es erzeugt
 * wurde und darf nicht zusammen mit einer anderen Otto-Instanz oder mit
 * Objekten anderen Otto-Instanzen verwendet werden.
 * Soll ein Sicherheitstoken von mehreren Otto-Instanzen verwendet werden, so
 * sind hierfür mehrere Zertifikatsobjekte zu erstellen: für jede Instanz eines.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, die das Zertifikatsobjekt verwenden soll.
 *
 * @param[in]  zertifikatsPfad
 *             Pfad zum Sicherheitstoken, folgende Angaben sind möglich:
 *             1. Clientseitig erzeugtes Zertifikat: <br>
 *               Pfad zum Verzeichnis, in dem sich die Zertifikats-Datei (.cer)
 *               und die Datei mit dem privaten Schlüssel (.p12) befinden.
 *               Diese Sicherheitstokens wurden mit EricMtCreateKey() bzw. EricCreateKey() erzeugt.
 *               Der Pfad zum Verzeichnis ist bei clientseitig erzeugten
 *               Zertifikaten relativ zum aktuellen Arbeitsverzeichnis oder absolut
 *               anzugeben.
 *             2. Software-Portalzertifikat: <br>
 *               Pfad zur Software-Zertifikatsdatei (i.d.R. mit der Endung .pfx).
 *               Der Pfad zur Datei ist bei Software-Zertifikaten relativ zum
 *               aktuellen Arbeitsverzeichnis oder absolut anzugeben.
 *             3. Sicherheitsstick: <br>
 *               Pfad zur Treiberdatei, siehe (*). Bitte beachten, dass der Treiber
 *               betriebssystemabhängig sein kann. Weitere Informationen in der
 *               Anleitung zum Sicherheitsstick oder unter
 *               https://www.sicherheitsstick.de.
 *             4. Signaturkarte: (**) <br>
 *               Pfad zur Treiberdatei, welcher einen Zugriff auf die
 *               Signaturkarte ermöglicht, siehe (*). Weitere Informationen in
 *               der Anleitung zur Signaturkarte.
 *             5. Elektronischer Personalausweis (nPA) oder Aufenthaltstitel (eAT): <br>
 *               Die URL des eID-Clients wie zum Beispiel der AusweisApp 2.
 *               In den meisten Fällen lautet diese URL: http://127.0.0.1:24727/eID-Client
 *               Optional kann auf die folgende Weise noch ein Testmerker angehängt werden:
 *               http://127.0.0.1:24727/eID-Client?testmerker=520000000
 *               Zu den verfügbaren Testmerkern siehe ERiC-Entwicklerhandbuch.pdf,
 *               Kap. "Testunterstützung bei der ERiC-Anbindung".
 *               \n\b Wichtig: Das Ad-hoc-Zertifikat, das in diesem Fall für den elektronischen
 *                        Personalausweis erzeugt wird, ist nur 24 Stunden gültig.
 *
 *             (*) Wird der Dateipfad eines Treibers angegeben, ist der Suchmechanismus zu
 *             beachten, mit dem das jeweilige Betriebssystem dynamische Bibliotheken lädt.
 *             Weitere Informationen sind der Systemdokumentation zu den Betriebssystemfunktionen
 *             LoadLibrary() (Windows) bzw. dlopen() (Linux, AIX und macOS) zu entnehmen.
 *
 *             (**) Bei Signaturkarten erfolgt eine PIN-Abfrage nicht beim Aufruf von
 *             OttoZertifikatOeffnen(), sondern erst, wenn beim Versand oder Empfang von Daten
 *             etwas authentifiziert oder signiert werden muss.
 *
 *             Pfade müssen auf Windows in der für Datei-Funktionen benutzten ANSI-Codepage,
 *             auf Linux, AIX und Linux Power in der für das Dateisystem benutzten Locale
 *             und auf macOS in der "decomposed form" von UTF-8 übergeben werden.
 *             Bitte weitere Betriebssystemspezifika bzgl. nicht erlaubter Zeichen in
 *             Pfaden und Pfadtrennzeichen beachten.
 *             Für Details zu Pfaden im ERiC siehe Entwicklerhandbuch Kapitel
 *             "Übergabe von Pfaden an ERiC API-Funktionen".
 *
 * @param[in]  zertifikatsPasswort
 *             Das Passwort oder die PIN des Sicherheitstokens.
 *             Bei Tokens, bei denen das Passwort oder die PIN nicht von der Anwendung
 *             übergeben, sondern separat über einen Treiber (z. B. von einem Kartenlesegerät)
 *             abgefragt wird, ist hier NULL zu übergeben.
 *
 * @param[out] zertifikat
 *             Handle auf das erstellte Zertifikatsobjekt
 *
 * @note       Aktuell werden von dieser Funktion nur clientseitig erzeugte Zertifikate (CEZs)
 *             unterstützt.
 *
 * @see
 *             - OttoZertifikatSchliessen()
 */
OttoStatusCode OttoZertifikatOeffnen(OttoInstanzHandle instanz,
                                     const byteChar *zertifikatsPfad,
                                     const byteChar *zertifikatsPasswort,
                                     OttoZertifikatHandle *zertifikat);

/**
 * @brief      Schließt das Otto-Zertifikatsobjekt zu einem Sicherheitstoken.
 * Anschließend darf das Zertifikatsobjekt nicht mehr verwendet werden.
 *
 * @param[in]  zertifikat
 *             Handle auf das Zertifikatsobjekt, das geschlossen werden soll.
 *
 * @see
 *             - OttoZertifikatOeffnen()
 */
OttoStatusCode OttoZertifikatSchliessen(OttoZertifikatHandle zertifikat);

/**
 * @brief      Erzeugt einen Rückgabepuffer und gibt ein Handle darauf zurück.
 *
 * Die von dieser Funktion erzeugten Rückgabepuffer werden verwendet, um
 * die Rückgabedaten von Otto-Funktionen (z. B. OttoEmpfangFortsetzen()
 * oder OttoVersandBeenden()) aufzunehmen.
 * Dazu wird das Rückgabepuffer-Handle für den Schreibvorgang
 * an die ausgebende Funktion übergeben.
 *
 * Zum Auslesen des von den API-Funktionen beschriebenen Puffers wird das
 * Rückgabepuffer-Handle an OttoRueckgabepufferInhalt() übergeben.
 * Ein einmal erzeugtes Rückgabepuffer-Handle kann für weitere nachfolgende
 * Aufrufe von Otto API-Funktionen wiederverwendet werden. Bei einer
 * Wiederverwendung eines Handles werden frühere Inhalte überschrieben.
 * Nach letztmaliger Verwendung muss jeder Rückgabepuffer mit
 * OttoRueckgabepufferFreigeben() freigegeben werden.
 *
 * Der Rückgabepuffer ist an die Otto-Instanz gebunden, für die er erzeugt
 * wurde und kann nicht zusammen mit einer anderen Otto-Instanz oder mit
 * Objekten anderen Otto-Instanzen verwendet werden.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, auf der diese Funktion ausgeführt werden soll.
 *
 * @param[out] rueckgabepuffer
 *             Zeiger auf das Handle des erzeugten Rückgabepuffers
 *
 * @see
 *             - OttoRueckgabepufferGroesse()
 *             - OttoRueckgabepufferInhalt()
 *             - OttoRueckgabepufferFreigeben()
 */
OttoStatusCode OttoRueckgabepufferErzeugen(OttoInstanzHandle instanz,
                                           OttoRueckgabepufferHandle *rueckgabepuffer);

/**
 * @brief      Gibt die Anzahl der im Rückgabepuffer
 *             enthaltenen Bytes zurück. Das abschließende Null-Byte wird
 *             nicht mitgezählt.
 *
 * @param[in]  rueckgabepuffer
 *             Das Handle des Rückgabepuffers
 *
 * @return
 *             - Anzahl der im Rückgabepuffer enthaltenen Bytes,
 *                 wenn ein gültiges Handle übergeben wird.
 *             - 0 sonst
 *
 * @see
 *             - OttoRueckgabepufferInhalt()
 *             - OttoRueckgabepufferErzeugen()
 *             - OttoRueckgabepufferFreigeben()
 */
uint64_t OttoRueckgabepufferGroesse(OttoRueckgabepufferHandle rueckgabepuffer);

/**
 * @brief      Gibt den Inhalt eines Rückgabepuffers zurück.
 *
 * Der zurückgegebene Zeiger verweist auf ein Byte-Array, das alle in
 * den Rückgabepuffer geschriebenen Bytes enthält.
 * Dieses Array existiert so lange im Speicher, bis der Rückgabepuffer
 * entweder (bei einer Wiederverwendung des Handles) erneut beschrieben
 * oder der Puffer explizit freigegeben wird.
 * Der Array wird immer von einem Null-Byte abgeschlossen. Wenn der
 * Rückgabepuffer keine weiteren Null-Bytes enthält, kann folglich der
 * Rückgabepufferinhalt bequem als null-terminierte Zeichenkette
 * interpretiert werden.
 *
 * @param[in]  rueckgabepuffer
 *             Das Handle des Rückgabepuffers, dessen Inhalt zurückgegeben werden soll.
 *
 * @return
 *             - Zeiger auf den Rückgabepufferinhalt, wenn ein gültiges Handle übergeben wird.
 *             - NULL sonst
 *
 * @see
 *             - OttoRueckgabepufferGroesse()
 *             - OttoRueckgabepufferErzeugen()
 *             - OttoRueckgabepufferFreigeben()
 */
const byteChar *OttoRueckgabepufferInhalt(OttoRueckgabepufferHandle rueckgabepuffer);

/**
 * @brief      Gibt einen Rückgabepuffer frei.
 *
 * Das Handle des Rückgabepuffers darf danach nicht weiter verwendet werden.
 * Es wird daher empfohlen, Handle-Variablen nach der Freigabe explizit auf
 * @c NULL zu setzen.
 *
 * @param[in]  rueckgabepuffer
 *             Handle auf den Rückgabepuffer, der freigegeben werden soll.
 *             Dieser Rückgabepuffer darf nicht bereits freigegeben worden sein.
 *
 * @see
 *            - OttoRueckgabepufferGroesse()
 *            - OttoRueckgabepufferInhalt()
 *            - OttoRueckgabepufferErzeugen()
 */
OttoStatusCode OttoRueckgabepufferFreigeben(OttoRueckgabepufferHandle rueckgabepuffer);

/**
 * @brief      Erzeugt ein Objekt zur Berechnung einer Datenprüfsumme, die Otto
 * zu Beginn einer Übermittlung an den OTTER-Server senden muss.
 *
 * Das Prüfsummenobjekt ist an die Otto-Instanz gebunden, für die es erzeugt
 * wurde und darf nicht zusammen mit einer anderen Otto-Instanz oder mit
 * Objekten anderen Otto-Instanzen verwendet werden.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, für die das Prüfsummenobjekt erzeugt werden soll.
 *
 * @param[out] pruefsumme
 *             Handle des erzeugten Prüfsummenobjekts
 *
 * @see
 *             - OttoPruefsummeAktualisieren()
 *             - OttoPruefsummeSignieren()
 *             - OttoPruefsummeFreigeben()
 */
OttoStatusCode OttoPruefsummeErzeugen(OttoInstanzHandle instanz, OttoPruefsummeHandle *pruefsumme);

/**
 * @brief      Aktualisiert die Prüfsumme über Daten.
 *             Eine Prüfsumme, die bereits signiert wurde, kann nicht mehr
 *             aktualisiert werden.
 *
 * @param[in,out] pruefsumme
 *                Handle der Prüfsumme, die aktualisiert werden soll.
 *
 * @param[in]  datenBlock
 *             Zeiger auf die Daten, über die die Prüfsumme aktualisiert werden soll.
 *
 * @param[in]  datenBlockGroesse
 *             Größe der Daten, über die die Prüfsumme aktualisiert werden soll, in Bytes.
 *
 * @see
 *             - OttoPruefsummeErzeugen()
 *             - OttoPruefsummeSignieren()
 *             - OttoPruefsummeFreigeben()
 * 
 * @return
 *             - ::OTTO_OK wenn die Prüfsumme erfolgreich aktualisiert werden konnte
 *             - ::OTTO_PRUEFSUMME_FINALISIERT wenn die Prüfsumme bereits signiert wurde
 */
OttoStatusCode OttoPruefsummeAktualisieren(OttoPruefsummeHandle pruefsumme,
                                           const byteChar *datenBlock,
                                           uint64_t datenBlockGroesse);

/**
 * @brief      Erstellt eine Signatur über eine Prüfsumme.
 *             Die Signierung der Prüfsumme ist nur dann möglich,
 *             wenn diese über die Mindestdatenmenge für eine Übermittlung
 *             an den OTTER-Server berechnet wurde. (20 MiB)
 *             Eine Prüfsumme kann nur einmalig signiert werden.
 *             Danach muß das Prüfsummenobjekt freigegeben werden.
 *
 * @param[in]  pruefsumme
 *             Handle der Prüfsumme, die signiert werden soll.
 *
 * @param[in]  zertifikat
 *             Handle des Sicherheitstoken, mit dem die Prüfsumme signiert werden soll.
 *
 * @param[out] rueckgabepuffer
 *             Handle des Rückgabepuffers, in den die signierte Prüfsumme
 *             geschrieben werden soll.
 *             Die signierte Prüfsumme wird als base64-codierte Zeichenfolge übergeben.
 *
 * @see
 *             - OttoPruefsummeErzeugen()
 *             - OttoPruefsummeAktualisieren()
 *             - OttoPruefsummeFreigeben()
 * 
 * @return
 *             - ::OTTO_OK wenn die Prüfsumme signiert werden konnte
 *             - ::OTTO_PRUEFSUMME_FINALISIERT wenn die Prüfsumme bereits signiert wurde
 *             - ::OTTO_VERSAND_GERINGE_DATENMENGE wenn die Prüfsumme über weniger Daten gebildet wurde als für den Versand an den OTTER-Server erforderlich sind
 *             - ::OTTO_ESIGNER_* bei Problemen mit dem übergebenen Zertifikat
 *             
 */
OttoStatusCode OttoPruefsummeSignieren(OttoPruefsummeHandle pruefsumme,
                                       OttoZertifikatHandle zertifikat,
                                       OttoRueckgabepufferHandle rueckgabepuffer);

/**
 * @brief      Gibt ein Prüfsummenobjekt frei.
 *
 * Das Prüfsummenobjekt darf danach nicht wieder verwendet werden.
 *
 * @param[in]  pruefsumme
 *             Handle des Prüfsummenobjekts, das freigegeben werden soll.
 *
 * @see
 *             - OttoPruefsummeErzeugen()
 *             - OttoPruefsummeAktualisieren()
 *             - OttoPruefsummeSignieren()
 */
OttoStatusCode OttoPruefsummeFreigeben(OttoPruefsummeHandle pruefsumme);

/**
 * @brief      Initialisiert einen Datenversand an den OTTER-Server.
 *
 * Das zurückgegebene Handle des Versandobjekts wird der Funktion
 * OttoVersandFortsetzen() übergeben, um Daten blockweise hochzuladen.
 * Sind alle Daten versendet, wird OttoVersandAbschliessen() aufgerufen, womit
 * der Versand abgeschlossen wird.
 * Zum Freigeben des Versandobjekts wird OttoVersandBeenden() aufgerufen.<br>
 * Bevor der Versand begonnen werden kann, muss eine Prüfsumme über alle zu
 * versendenen Daten gebildet (siehe OttoPruefsummeErzeugen()) und mit
 * OttoPruefsummeSignieren() signiert werden.
 *
 * @note       Wurde Otto vor dem Aufruf dieser Funktion für einen Proxy mit OttoProxyKonfigurationSetzen()
 *             konfiguriert, wird der Versand über den Proxy durchgeführt. Die Proxy-Konfiguration wird
 *             intern an dem Versandobjekt gespeichert und spätere Aufrufe von OttoProxyKonfigurationSetzen()
 *             haben keinen Einfluss auf den bereits begonnenen Versand.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, auf der diese Funktion ausgeführt werden soll.
 *
 * @param[in]  signiertePruefsumme
 *             Signierte Prüfsumme über die Gesamtheit der Daten, die in diesem Versand
 *             versendet werden sollen.
 *             Die signierte Prüfsumme wird als base64-codierte, nullterminierte Zeichenfolge erwartet,
 *             wie sie von OttoPruefsummeSignieren() zurückgeliefert wird.
 *
 * @param[in]  herstellerId
 *             Hersteller-ID des Softwareproduktes
 *
 * @param[out] versand
 *             Handle auf das Versandobjekt. Im Fehlerfall wird kein Versandobjekt erzeugt.
 *
 * @see
 *             - OttoVersandFortsetzen()
 *             - OttoVersandAbschliessen()
 *             - OttoVersandBeenden()
 *             - OttoPruefsummeSignieren()
 *             - OttoProxyKonfigurationSetzen()
 */
OttoStatusCode OttoVersandBeginnen(OttoInstanzHandle instanz,
                                   const byteChar *signiertePruefsumme,
                                   const byteChar *herstellerId,
                                   OttoVersandHandle *versand);

/**
 * @brief      Versendet einen Datenblock an den OTTER-Server.
 *
 * Otto liest den übergebenen Datenblock ein und versendet ihn an den OTTER-Server.
 * Wenn @c OTTO_OK zurückgegeben wird, kann diese Funktion erneut mit einem weiteren
 * Datenblock aufgerufen werden. Dies ist zu wiederholen, bis Otto alle zu diesem Versand
 * gehörigen Daten erhalten hat. Falls nicht @c OTTO_OK zurückgegeben wird, ist der
 * Versand fehlgeschlagen.<br>
 * Ist das Ende der Daten erreicht, muss OttoVersandAbschliessen() aufgerufen werden.
 *
 * @param[in]  versand
 *             Ein mit OttoVersandBeginnen() erzeugtes Handle
 *
 * @param[in]  datenBlock
 *             Zeiger auf die zu versendenen Daten. Falls @c NULL wird der Aufruf ignoriert.
 *
 * @param[in]  datenBlockGroesse
 *             Größe des Arrays @p datenBlock in Bytes. Falls 0 wird der Aufruf ignoriert.
 *
 * @return
 *             - ::OTTO_OK im Erfolgsfall
 *             - ::OTTO_TRANSFER_UNAUTHORIZED
 *             - ::OTTO_TRANSFER_CONNECTSERVER
 *             - ::OTTO_VERSAND_ABGESCHLOSSEN falls OttoVersandAbschliessen() bereits aufgerufen wurde
 *
 * @see
 *             - OttoVersandAbschliessen()
 */
OttoStatusCode OttoVersandFortsetzen(OttoVersandHandle versand,
                                     const byteChar *datenBlock,
                                     uint64_t datenBlockGroesse);

/**
 * @brief      Schließt einen Versand ab und gibt die Objekt-ID zurück
 *
 * Mit dieser Funktion wird das Ende der Daten gekennzeichnet und der Datenversand abgeschlossen.<br> 
 * Im Erfolgsfall wird die vom OTTER-Server vergebene Objekt-ID zurückgegeben, über die die
 * versendeten Daten bei OTTER referenziert werden.
 * 
 * @param[in]  versand
 *             Ein mit OttoVersandBeginnen() erzeugtes Handle
 *
 * @param[out] objektId
 *             Handle des Rückgabepuffers, in den die Objekt-ID geschrieben werden soll.
 *
 * @return
 *             - ::OTTO_OK im Erfolgsfall
 *             - ::OTTO_TRANSFER_UNAUTHORIZED
 *             - ::OTTO_TRANSFER_CONNECTSERVER
 *             - ::OTTO_VERSAND_GERINGE_DATENMENGE
 *             - ::OTTO_VERSAND_ABGESCHLOSSEN falls OttoVersandAbschliessen() bereits aufgerufen wurde
 *
 */
OttoStatusCode OttoVersandAbschliessen(OttoVersandHandle versand,
                                       OttoRueckgabepufferHandle objektId);

/**
 * @brief      Gibt ein Versandobjekt frei.
 *
 * Das Versandobjekt darf danach nicht wieder verwendet werden.
 *
 * @param[in]  versand
 *             Handle des Versandobjekts, das freigegeben werden soll.
 *
 * @see
 *             - OttoVersandBeginnen()
 *             - OttoVersandFortsetzen()
 *             - OttoVersandAbschliessen()
 */
OttoStatusCode OttoVersandBeenden(OttoVersandHandle versand);

/**
 * @brief      Initialisiert eine Datenabholung vom OTTER-Server.
 *
 * Das zurückgegebene Handle des Empfangsobjekts wird der Funktion
 * OttoEmpfangFortsetzen() übergeben, um Daten blockweise abzuholen.
 * Sind alle Daten abgeholt, wird OttoEmpfangBeenden() aufgerufen, womit
 * das Empfangsobjekt wieder freigegeben wird.
 *
 * @note       Wurde eine Otto-Instanz vor dem Aufruf dieser Funktion mit OttoProxyKonfigurationSetzen()
 *             für einen Proxy konfiguriert, wird der Empfang über den Proxy durchgeführt.
 *             Die Proxy-Konfiguration wird intern an dem Empfangsobjekt gespeichert und spätere Aufrufe
 *             von OttoProxyKonfigurationSetzen() haben keinen Einfluss auf den bereits begonnenen Empfang.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, auf der diese Funktion ausgeführt werden soll.
 *
 * @param[in]  objektId
 *             ID des Objekts, das vom OTTER-Server abgeholt werden soll.
 *
 * @param[in]  zertifikat
 *             Handle auf ein Zertifikatsobjekt
 *
 * @param[in]  herstellerId
 *             Hersteller-ID des Softwareproduktes
 *
 * @param[out] empfang
 *             Handle auf das Empfangsobjekt. Im Fehlerfall wird kein Empfangobjekt erzeugt.
 *
 * @see
 *             - OttoEmpfangFortsetzen()
 *             - OttoEmpfangBeenden()
 *             - OttoProxyKonfigurationSetzen()
 *
 */
OttoStatusCode OttoEmpfangBeginnen(OttoInstanzHandle instanz,
                                   const byteChar *objektId,
                                   OttoZertifikatHandle zertifikat,
                                   const byteChar *herstellerId,
                                   OttoEmpfangHandle *empfang);

/**
 * @brief      Initialisiert eine Datenabholung vom OTTER-Server mit Angabe eines Abholzertifikats
 *
 * Das zurückgegebene Handle des Empfangsobjekts wird der Funktion
 * OttoEmpfangFortsetzen() übergeben, um Daten blockweise abzuholen.
 * Sind alle Daten abgeholt, wird OttoEmpfangBeenden() aufgerufen, womit
 * das Empfangsobjekt wieder freigegeben wird.
 * 
 * Ein wichtiger Unterschied zu OttoEmpfangBeginnen() besteht darin, dass der OTTER-Server die Daten auf
 * den in @p abholzertifikat enthaltenen öffentlichen Schlüssel umschlüsselt. Die Daten werden vom Otto
 * nicht entschlüsselt und OttoEmpfangFortsetzen() gibt lediglich die verschlüsselten Daten zurück.
 *
 * @note       Wurde eine Otto-Instanz vor dem Aufruf dieser Funktion mit OttoProxyKonfigurationSetzen()
 *             für einen Proxy konfiguriert, wird der Empfang über den Proxy durchgeführt.
 *             Die Proxy-Konfiguration wird intern an dem Empfangsobjekt gespeichert und spätere Aufrufe
 *             von OttoProxyKonfigurationSetzen() haben keinen Einfluss auf den bereits begonnenen Empfang.
 *
 * @param[in]  instanz
 *             Handle der Otto-Instanz, auf der diese Funktion ausgeführt werden soll.
 *
 * @param[in]  objektId
 *             ID des Objekts, das vom OTTER-Server abgeholt werden soll.
 *
 * @param[in]  zertifikat
 *             Handle auf ein Zertifikatsobjekt
 *
 * @param[in]  herstellerId
 *             Hersteller-ID des Softwareproduktes
 *
 * @param[in]  abholzertifikat
 *             Base64-kodierter Teil eines X.509-v3-Zertifikats im PEM-Format
 * 
 * @param[out] empfang
 *             Handle auf das Empfangsobjekt. Im Fehlerfall wird kein Empfangobjekt erzeugt.
 *
 * @see
 *             - OttoEmpfangFortsetzen()
 *             - OttoEmpfangBeenden()
 *             - OttoProxyKonfigurationSetzen()
 *
 */
OttoStatusCode OttoEmpfangBeginnenAbholzertifikat(OttoInstanzHandle instanz,
                                                  const byteChar* objektId,
                                                  OttoZertifikatHandle zertifikat,
                                                  const byteChar* herstellerId,
                                                  const byteChar* abholzertifikat,
                                                  OttoEmpfangHandle* empfang);

/**
 * @brief      Empfängt einen Datenblock vom OTTER-Server
 *
 * Otto empfängt Daten vom OTTER-Server und gibt sie blockweise an den Aufrufer zurück.
 * Wird @c OTTO_OK zurückgegeben, kann diese Funktion erneut aufgerufen werden und weitere
 * Datenblöcke empfangen werden. Werden leere Daten zurückgegeben, ist der Empfang beendet
 * und alle Daten wurden empfangen. Dann muss OttoEmpfangBeenden() aufgerufen werden.
 *
 * @param[in]  empfang
 *             Ein mit OttoEmpfangBeginnen() erzeugtes Handle.
 *
 * @param[out] datenBlock
 *             Rückgabepuffer mit allen oder einem Teil der empfangenen Daten. Falls leer, ist der Empfang beendet.
 *             Der Inhalt des Rückgabepuffers darf nicht als null-terminierte Zeichenkette interpretiert werden, da die
 *             empfangenen Daten weitere Null-Bytes enthalten können.
 *
 * @return
 *             - ::OTTO_OK
 *             - ::OTTO_TRANSFER_UNAUTHORIZED
 *             - ::OTTO_TRANSFER_NOT_FOUND
 *
 * @see
 *             - OttoEmpfangBeenden()
 */
OttoStatusCode OttoEmpfangFortsetzen(OttoEmpfangHandle empfang,
                                     OttoRueckgabepufferHandle datenBlock);

/**
 * @brief      Gibt das Empfangsobjekt wieder frei.
 *
 * Das Empfangsobjekt darf nach diesem Aufruf nicht mehr verwendet werden.
 * Wird diese Funktion aufgerufen, bevor OttoEmpfangFortsetzen() einen leeren Rückgabepuffer
 * zurückgegeben hat, muss angenommen werden, dass die bis dahin empfangenen Daten unvollständig sind.
 *
 * @param[in]  empfang
 *             Ein mit OttoEmpfangBeginnen() erzeugtes Handle
 *
 * @return
 *             - ::OTTO_OK
 *             - ::OTTO_EMPFANG_VORZEITIG_BEENDET
 */
OttoStatusCode OttoEmpfangBeenden(OttoEmpfangHandle empfang);

/**
 * @brief      Konfiguriert eine Otto-Instanz für einen Proxy.
 *
 * Damit eine Otto-Instanz ihre Internetverbindungen über einen
 * Proxy aufbaut, muss ihr die Proxy-Konfiguration über diese Methode
 * mitgeteilt werden.
 * Die Konfiguration gilt dann für alle Verbindungen der Instanz nach
 * außen, d.h. für die Verbindungen zu den OTTER-Servern ebenso wie
 * für Verbindungen zum ELSTER-eID-Server bei der Verwendung eines
 * elektronischen Personalausweises oder Aufenthaltstitels.
 *
 * @param[in]  instanz
 *             Die Otto-Instanz, für die die Konfiguration gelten soll.
 *
 * @param[in]  proxyKonfiguration
 *             Die Proxy-Konfiguration, die von der Otto-Instanz verwendet werden soll.
 *             Wenn hier @c NULL übergeben wird, verwendet die Otto-Instanz keinen Proxy.
 *
 * @see
 *             - OttoProxyKonfiguration
 */
OttoStatusCode OttoProxyKonfigurationSetzen(OttoInstanzHandle instanz,
                                            const OttoProxyKonfiguration* proxyKonfiguration);

/**
 * @brief      Gibt die Version der Otto-Bibliothek zurück.
 *
 * @note       Die Version der Otto-Bibliothek ist nicht zwingend gleich der Version
 *             des ERiC-Auslieferungspaketes, sondern kann davon abweichen.
 */
OttoStatusCode OttoVersion(OttoRueckgabepufferHandle rueckgabepuffer);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* OTTO_H_ */
