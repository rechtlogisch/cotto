#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include "include/otto.h"

int error(const std::string message, const OttoStatusCode code)
{
    std::cerr << "[ERROR] " << message << std::endl;
    std::cerr << "[CODE]  " << code << std::endl;
    return code;
}

int downloadError(const OttoStatusCode code)
{
    const char *message;
    
    switch (code) {
        case OTTO_TRANSFER_UNAUTHORIZED:
            message = "The client is not allowed to use the API.";
            break;
        case OTTO_TRANSFER_NOT_FOUND:
            message = "The OTTER server did not find the object";
            break;
        default:
            message = "Error occurred while downloading. Check otto.log for details.";
            break;
    } 
    return error(message, code);
}

class Cotto {
    private:
        OttoInstanzHandle instance;
        OttoZertifikatHandle certificateHandle;
        OttoRueckgabepufferHandle contentHandle;
        OttoEmpfangHandle downloadHandle;
    public:
        Cotto(const char* pathLog, const char* pathCertificate, const char* certificatePassword) {
            // Create instance
            const OttoStatusCode statusCodeInstanceCreate = OttoInstanzErzeugen(pathLog, NULL, NULL, &instance);
            if (statusCodeInstanceCreate != OTTO_OK) {
                error("Could not create an Otto instance. Check otto.log for details.", statusCodeInstanceCreate);
            }

            // Open certificate
            const OttoStatusCode statusCodeCertificateOpen = OttoZertifikatOeffnen(instance, pathCertificate, certificatePassword, &certificateHandle);
            if (statusCodeCertificateOpen != OTTO_OK) {
                error("Could not open certificate: " + std::string(pathCertificate), statusCodeCertificateOpen);
            }
            std::cout << "[INFO]  Using certificate path: " << pathCertificate << std::endl;
        }

        ~Cotto() {
            // End download
            if (downloadHandle != NULL) {
                const OttoStatusCode statusCodeDownloadEnd = OttoEmpfangBeenden(downloadHandle);
                if (statusCodeDownloadEnd != OTTO_OK) {
                    error("Could not end download.", statusCodeDownloadEnd);
                }
            }

            // Close certificate
            if (certificateHandle != NULL) {
                const OttoStatusCode statusCodeCertificateClose = OttoZertifikatSchliessen(certificateHandle);
                if (statusCodeCertificateClose != OTTO_OK) {
                    error("Could not close certificate handle", statusCodeCertificateClose);
                }
            }

            // Release content buffer
            if (contentHandle != NULL) {
                const OttoStatusCode statusCodeContentRelease = OttoRueckgabepufferFreigeben(contentHandle);
                if (statusCodeContentRelease != OTTO_OK) {
                    error("Could not release content handle.", statusCodeContentRelease);
                }
            }

            // Destroy instance
            if (instance != NULL) {
                const OttoStatusCode statusCodeInstanceDestroy = OttoInstanzFreigeben(instance);
                if (statusCodeInstanceDestroy != OTTO_OK) {
                    error("Could not destroy the Otto instance. Check otto.log for details.", statusCodeInstanceDestroy);
                }
            }
        }

        int workflow(const char* objectUuid, const char* developerId, const char* fileExtension, const char* pathDownload) {
            // Start download
            const OttoStatusCode statusCodeDownloadStart = OttoEmpfangBeginnen(instance, objectUuid, certificateHandle, developerId, &downloadHandle);
            if (statusCodeDownloadStart != OTTO_OK) {
                return error("Could not start download. Check otto.log", statusCodeDownloadStart);
            }

            // Create content buffer
            const OttoStatusCode statusCodeContentHandleCreate = OttoRueckgabepufferErzeugen(instance, &contentHandle);
            if (statusCodeContentHandleCreate != OTTO_OK) {
                return error("Could not create handle for content.", statusCodeContentHandleCreate);
            }

            const std::string filepath = std::string(pathDownload) + "/" + std::string(objectUuid) + "." + std::string(fileExtension);
            FILE *file = fopen(filepath.c_str(), "ab");  // Open file for appending in binary mode
            if (!file) {
                std::cerr << "Failed to open file: " << filepath << std::endl;
                return 6;
            }

            // Continue download
            bool downloadContinue = true;
            OttoStatusCode statusCodeDownloadContinue = OTTO_UNBEKANNTER_FEHLER;
            while (downloadContinue == true) {
                statusCodeDownloadContinue = OttoEmpfangFortsetzen(downloadHandle, contentHandle);
                if (statusCodeDownloadContinue != OTTO_OK) {
                    downloadContinue = false;
                    break;
                }

                uint64_t contentSize = OttoRueckgabepufferGroesse(contentHandle);
                if (contentSize <= 0) {
                    downloadContinue = false;
                    break;
                }
                std::cout << "[INFO]  Downloaded: " << contentSize << " bytes" << std::endl;
                const byteChar* contentBlock = OttoRueckgabepufferInhalt(contentHandle);
                size_t contentWrite = fwrite(contentBlock, 1, contentSize, file);
                if (contentWrite != contentSize) {
                    std::cerr << "Failed to write to file: " << filepath << std::endl;
                    fclose(file);
                    break;
                }
            }

            if (fclose(file) != 0) {
                std::cerr << "Failed to close file: " << filepath << std::endl;
                return 7;
            }

            if (statusCodeDownloadContinue != OTTO_OK) {
                unlink(filepath.c_str());
                return downloadError(statusCodeDownloadContinue);  
            }

            std::cout << "[INFO]  Downloaded content saved in: " << filepath << std::endl;
            return EXIT_SUCCESS;
        }
};

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "" << argv[0] << std::endl;
        std::cerr << "  -u objectUuid\t\tUUID of object to download (mandatory)" << std::endl;
        std::cerr << "  -e extension\t\tSet filename extension of downloaded content [default: \"txt\"]" << std::endl;
        std::cerr << "  -p password\t\tPassword for certificate [default: \"123456\"]" << std::endl;
        std::cerr << "  -f\t\t\tForce file overwriting [default: false]" << std::endl;
        return 1;
    }

    int option;
    const char* objectUuid = NULL;
    const char* fileExtension = "txt";
    const char* certificatePassword = "123456";
    bool forceOverwrite = false;
    while ((option = getopt(argc, argv, "u:e:p:f")) != -1) {
        switch (option) {
            case 'u':
                objectUuid = optarg;
                break;
            case 'e':
                fileExtension = optarg;
                break;
            case 'p':
                certificatePassword = optarg;
                break;
            case 'f':
                forceOverwrite = true;
                break;
            default:
                std::cerr << "Invalid option: -" << static_cast<char>(optopt) << std::endl;
                return 2;
        }
    }

    if (objectUuid == NULL) {
        std::cerr << "Object UUID is missing. Please provide it with the -u flag." << std::endl;
        return 3;
    }

    const char* pathDownload = ".";
    const char* envPathDownload = getenv("PATH_DOWNLOAD");
    if (envPathDownload != NULL) {
        pathDownload = envPathDownload;
    }

    // Check if file already exists
    const std::string filepath = std::string(pathDownload) + "/" + std::string(objectUuid) + "." + std::string(fileExtension);
    if (access(filepath.c_str(), F_OK) != -1) {
        if (forceOverwrite) {
            unlink(filepath.c_str());
        } else {
            char userChoice;
            std::cout << "File: " << filepath << " already exists. Do you want to overwrite it? (y/n)" << std::endl;
            std::cin >> userChoice;
            if (userChoice == 'y' || userChoice == 'Y') {
                unlink(filepath.c_str());
            } else {
                std::cerr << "File: " << filepath << " was not overwritten. Stopping." << std::endl;
                return 5;
            }
        }
    }

    const char* developerId = getenv("DEVELOPER_ID");
    if (developerId == NULL || std::string(developerId) == "") {
        std::cerr << "DEVELOPER_ID environment variable missing. Please set it accordingly." << std::endl;
        return 4;
    }

    const char* pathCertificate = "certificate/test-softorg-pse.pfx";
    const char* envPathCertificate = getenv("PATH_CERTIFICATE");
    if (envPathCertificate != NULL) {
        pathCertificate = envPathCertificate;
    }

    const char* pathLog = ".";
    const char* envPathLog = getenv("PATH_LOG");
    if (envPathLog != NULL) {
        pathLog = envPathLog;
    }

    Cotto cotto(pathLog, pathCertificate, certificatePassword);
    return cotto.workflow(objectUuid, developerId, fileExtension, pathDownload);
}
