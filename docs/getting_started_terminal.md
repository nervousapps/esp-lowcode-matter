# Terminal

* [Terminal](#terminal)
* [Get Started: Terminal](#get-started-terminal)
* [Setup Environment](#setup-environment)
* [Select Product](#select-product)
* [Prepare Device](#prepare-device)
* [Upload Configuration](#upload-configuration)
* [Upload Code](#upload-code)

If you don't want to use [Codespaces](../README.md#start-codespace) (recommended), you can manually setup the development environment on your local machine.

You can either use the terminal directly (continue reading below) or also use [VS Code](./getting_started_vscode.md) to setup the development environment.

## Get Started: Terminal

### Setup Environment

Clone the repositories and install the tools and dependencies.

```sh
git clone -b release/v5.3 https://github.com/espressif/esp-idf.git --recursive
cd esp-idf
export ESP_IDF_PATH=$(pwd)/esp-idf
./install.sh
. ./export.sh
cd ..
```

```sh
git clone -b main https://github.com/espressif/esp-amp.git --recursive
cd esp-amp
export ESP_AMP_PATH=$(pwd)/esp-amp
git submodule update --init --recursive
cd ..
```

```sh
git clone -b main https://github.com/espressif/esp-lowcode-matter.git --recursive
cd esp-lowcode-matter
git submodule update --init --recursive
./install.sh
. ./export.sh
cd ..
```

> Note: Change the below commands according to the product that you want to create.

### Select Product

```sh
export SELECTED_PRODUCT=light_cw_pwm
cd $LOW_CODE_PATH/products/$SELECTED_PRODUCT
```

### Prepare Device

>(Note: Ensure that your hardware is properly set up and that USB port permissions are correctly configured. Refer [Port Permissions](./hardware_setup.md#port-permissions-for-usb-to-serial-converters) for OS specific instructions.)

Erase the flash on the device and flash the pre-built binaries to the device. This only needs to be done once for each device.

```sh
cd $LOW_CODE_PATH/pre_built_binaries
esptool.py erase_flash
esptool.py write_flash $(cat flash_args)
```

### Upload Configuration

Generate and flash the required device certificates and the qr code for the device. This only needs to be done once for each device.

First, get the mac address of the device by running the following command.

```sh
cd $LOW_CODE_PATH/tools/mfg
esptool.py chip_id
```

It should be in the format of `MAC: 01:23:45:67:89:ab:cd:ef`. Get the mac address in the format of `0123456789ABCDEF` (without colons, and in uppercase).

Then, generate the configuration for the device.

```sh
cd $LOW_CODE_PATH/tools/mfg
export MAC_ADDRESS=<mac_address>
./mfg_low_code.sh $LOW_CODE_PATH/products/$SELECTED_PRODUCT esp32c6 $MAC_ADDRESS
esptool.py write_flash 0xD000 $LOW_CODE_PATH/products/$SELECTED_PRODUCT/configuration/output/$MAC_ADDRESS/${MAC_ADDRESS}_esp_secure_cert.bin 0x1F2000 $LOW_CODE_PATH/products/$SELECTED_PRODUCT/configuration/output/$MAC_ADDRESS/${MAC_ADDRESS}_fctry.bin
```

The QR code is generated at `$LOW_CODE_PATH/products/$SELECTED_PRODUCT/configuration/output/$MAC_ADDRESS/qr_code.png`. Open it separately.

### Upload Code

Build: Compile the application code.

```sh
cd $LOW_CODE_PATH/products/$SELECTED_PRODUCT
idf.py set-target esp32c6
idf.py build
```

Flash: Upload it to the device

```sh
esptool.py write_flash 0x20C000 build/$SELECTED_PRODUCT.bin
```

Console: Start the device console to view the logs

```sh
python3 -m esp_idf_monitor
```

## Related Documents

* [Device Setup](./device_setup.md)
* [Create LowCode Product](./create_product.md)
* [Product Configuration](./product_configuration.md)
* [All Documents](./all_documents.md)
