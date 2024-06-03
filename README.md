![Recht logisch cotto banner image](rechtlogisch-cotto-banner.png)

# cotto

> Unofficial C++ Otto download demo

A working example of the Otto library implementation in C++.

Notice: This demo is **not intended for productive usage**!

ELSTER introduced on May 27th, 2024, a new library called Otto, which can be used to download objects from OTTER (Object Storage in ELSTER). The reason why ELSTER introduced the project and library is, that the current solution hit its limits. Otto was introduced together with Datenabholung v30. The currently available way of data retrieval with ERiC will be replaced client-side in ERiC version 41 (end November 2024; only Datenabholung v30 will be available) and server-side with the planned yearly minimal version increase mid-April 2025 (exact date TBA in 2025; only ERiC >= v41 can be used after that date).

## Usage

```bash
./cotto
  -u objectUuid         UUID of object to download (mandatory)
  -e extension          Set filename extension of downloaded content [default: "txt"]
  -p password           Password for certificate [default: "123456"]
  -f                    Force file overwriting [default: false]
```

Examples:

```bash
cotto -u 2b884e20-779d-4094-a1de-8304a6fc00c9 # ESB 2014
cotto -u 131517ff-4516-418a-9167-f7e35fb32864 # ELOProtokoll
```

Notice: The code and scripts have been tested so far only on Linux and macOS. Windows is pending tests.

Hint: Currently, a limited list of object UUIDs is available. You can get a list of them using `PostfachAnfrage` with the Test-CEZ.

## Vendor

You need the official ELSTER Otto library. Download the ERiC package v40 for your platform from the [ELSTER developer area](https://www.elster.de/elsterweb/entwickler/infoseite/eric), unzip it and place it at a desired path. Feel free to place it in `./vendor/`. Currently, you only need one library called `otto` (platform dependent naming: `(lib)otto.{so,dylib,dll}`), as Otto supports only client-side generated certificates (German abbreviation: CEZ).

Notice: The ERiC package, inespecially the included there libraries are subject to a seperate license agreement (presented before download in the ELSTER developer area and included in the ERiC package itself).

Hint: Choose the right library for the platform you compile and run on.

## Compile

You can compile the code with:

```bash
make
```

Hints: provide `PATH_VENDOR` environment variable to point the path where the ELSTER `otto` library is placed on your system or place the needed libraries in `./vendor/`.

## Run

Follow the steps to run `cotto` locally:

```bash
# Clone repository
git clone git@github.com:rechtlogisch/cotto.git

# Change to directory with source code
cd cotto

# Retrieve CEZ certificate to "CEZ" subdirectory
./get-test-cez-certificate.sh

# Compile statically linked to Otto library placed in PATH_VENDOR
PATH_VENDOR="/path/to/otto-library/" make

# Run `cotto`, provide objectUuid with -u option and your DEVELOPER_ID inline
DEVELOPER_ID="_____" ./cotto -u 2b884e20-779d-4094-a1de-8304a6fc00c9
```

Notice: You should set your five-digit Developer-ID (German: Hersteller-ID) as the environment variable `DEVELOPER_ID`. You could source it from for example `.env` or pass it inline to `cotto`, as shown in the steps above.

Hint: The downloaded result will be saved in the same directory as `cotto`, unless you provide a different `PATH_DOWNLOAD`.

## Environment variables

All supported environment variables are listed in [`.env.example`](.env.example). Feel free to copy them to `.env`, adjust accordingly and source for usage.

## Docker

A simple [Dockerfile](Dockerfile) is included. You can use `make docker-build` and `make docker-cotto` to build and run `cotto` in a container.

Hint: Set `PATH_DOWNLOAD` and `PATH_LOG` environment variables to expose data outside the container.

## Changelog

Please see [CHANGELOG](CHANGELOG.md) for more information on what has changed recently.

## Contributing

Please see [CONTRIBUTING](https://github.com/rechtlogisch/.github/blob/main/CONTRIBUTING.md) for details.

## Security Vulnerabilities

If you discover any security-related issues, please email open-source@rechtlogisch.de instead of using the issue tracker.

## Credits

- [Krzysztof Tomasz Zembrowski](https://github.com/rechtlogisch)
- [All Contributors](../../contributors)

## License

The MIT License (MIT). Please see [License File](LICENSE.md) for more information.

The ERiC package, especially libraries, is not included in this repository and is subject to a separate license agreement. Please see the [ELSTER developer area](https://www.elster.de/elsterweb/entwickler/infoseite/eric) or the lizenz.pdf included in the ERiC package for more information.

## Disclaimer

This demo was developed by [RL Recht logisch GmbH & Co. KG](https://rechtlogisch.de/impressum/) and should be used only for test purposes.

ELSTER is a registered trademark of the Freistaat Bayern, represented by the Bayerische Staatsministerium der Finanzen.
