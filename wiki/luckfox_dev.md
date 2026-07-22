# Luckfox pico development
Para crear el sistema operativo con los drivers, librerias y programas que necesitamos, es necesario utilizar el [SDK](https://github.com/LuckfoxTECH/luckfox-pico) que nos provee Luckfox. Esta pensado para generar una imagen de sistema Buildroot. Una sistema a medida que cabe en la pequeña memoria SPI y no es necesario una SD.

## Download SDK
Clonar el repo en una carpeta siguiendo su guia de instalación.

## Añadir el driver para la pantalla LCD
> Based on: [gc9a01-for-luckfox-pico](https://github.com/huxiangjs/gc9a01-for-luckfox-pico)  
> Dudo que todavia funcione tal y como el indica de aplicar el parche, pero lo haremos manualmente a continuacion:

El fichero donde añadir el **overlay** es:
`sysdrv/source/kernel/arch/arm/boot/dts/rv1103g-luckfox-pico.dts`

Tenemos que cambiar la sección del SPI de:
```
/**********SPI**********/
/* SPI0_M0 */
&spi0 {
	status = "disabled";
	spidev@0 {
		spi-max-frequency = <50000000>;
	};
	fbtft@0 {
		spi-max-frequency = <50000000>;
	};
};
```
A:
```
&spi0 {
	status = "okay";

	#address-cells = <1>;
	#size-cells = <0>;

	gc9a01@0 {
		compatible = "ilitek,ili9340";
		reg = <0>;

		reset-gpios = <&gpio1 RK_PC4 1>;
		dc-gpios = <&gpio1 RK_PC7 0>;
		led-gpios = <&gpio1 RK_PC6 0>;

		spi-max-frequency = <40000000>;
		buswidth = <8>;
		width = <240>;
		height = <240>;
		rotate = <0>;
		fps = <75>;
		bgr;

		init = <
			0x01000011 /* Sleep mode OFF */
			0x02000078 /* Delay 120ms */
			0x010000EF /* Inter register enable 2 */
			0x010000EB 0x14
			/* BEGIN set inter_command HIGH */
			0x010000FE /* Inter register enable 1 */
			0x010000EF /* Inter register enable 2 */
			/* END set inter_command HIGH */
			0x010000EB 0x14
			0x01000084 0x40
			0x01000085 0xFF
			0x01000086 0xFF
			0x01000087 0xFF
			0x01000088 0x0A
			0x01000089 0x21
			0x0100008A 0x00
			0x0100008B 0x80
			0x0100008C 0x01
			0x0100008D 0x01
			0x0100008E 0xFF
			0x0100008F 0xFF
			0x010000B6 0x00 0x00 /* Display function control */
			0x01000036 0x08 /* Memory access control */
			0x0100003A 0x05 /* Pixel format */
			0x01000090 0x08 0x08 0x08 0x08
			0x010000BD 0x06
			0x010000BC 0x00
			0x010000FF 0x60 0x01 0x04
			0x010000C3 0x13 /* Voltage regulator 1a */
			0x010000C4 0x13 /* Voltage regulator 1b */
			0x010000C9 0x22 /* Voltage regulator 2a */
			0x010000BE 0x11
			0x010000E1 0x10 0x0E
			0x010000DF 0x21 0x0c 0x02
			0x010000F0 0x45 0x09 0x08 0x08 0x26 0x2A /* Set gamma1 */
			0x010000F1 0x43 0x70 0x72 0x36 0x37 0x6F /* Set gamma2 */
			0x010000F2 0x45 0x09 0x08 0x08 0x26 0x2A /* Set gamma3 */
			0x010000F3 0x43 0x70 0x72 0x36 0x37 0x6F /* Set gamma4 */
			0x010000ED 0x1B 0x0B
			0x010000AE 0x77
			0x010000CD 0x63
			0x01000070 0x07 0x07 0x04 0x0E 0x0F 0x09 0x07 0x08 0x03
			0x010000E8 0x34 /* Frame rate */
			0x01000062 0x18 0x0D 0x71 0xED 0x70 0x70 0x18 0x0F 0x71 0xEF 0x70 0x70
			0x01000063 0x18 0x11 0x71 0xF1 0x70 0x70 0x18 0x13 0x71 0xF3 0x70 0x70
			0x01000064 0x28 0x29 0xF1 0x01 0xF1 0x00 0x07
			0x01000066 0x3C 0x00 0xCD 0x67 0x45 0x45 0x10 0x00 0x00 0x00
			0x01000067 0x00 0x3C 0x00 0x00 0x00 0x01 0x54 0x10 0x32 0x98
			0x01000074 0x10 0x85 0x80 0x00 0x00 0x4E 0x00
			0x01000098 0x3e 0x07
			0x01000035 /* Tearing effect ON */
			0x01000021 /* Display inversion ON */
			0x01000011 /* Sleep mode OFF */
			0x0200000C /* Delay 12ms */
			0x01000029 /* Display ON */
			0x02000014 /* Delay 20ms */
			>;
	};
};
```

En el fichero `/sysdrv/source/kernel/arch/arm/configs/luckfox_rv1106_linux_defconfig` buscamos:
```
CONFIG_FB_TFT=y
CONFIG_FB_TFT_ST7735R=y
CONFIG_FB_TFT_ST7789V=y
```
y lo dejamos como:
```
CONFIG_FB_TFT=y
CONFIG_FB_TFT_ILI9340=y
```
## Compilar el programa de Linux-Scope














Referncia: https://wiki.luckfox.com/Luckfox-Pico-RV1106/opencv-mobile/  
Se utiliza una version muy ligera y preparada para esta placa.

El programa de `linux-scope` se compila en tu propio PC (no en el Luckfox), haciendo compilacion cruzada y utilizando esta libreria. 





## Configure SC3336 camera 
Creo que es mejor tener muchos más fps, por encima de mas calidad/zoom y optico. Al fin y al cabo queremos ver como si fuera una mira convencional, sin delay.  
Para ello, me he basado en la config de https://github.com/LuckfoxTECH/luckfox-pico/issues/67 para hacerlo antes de generar la imagen.

1. Substituye `sc3336_CMK-OT2119-PC1_30IRC-F16.json` dela ruta `/luckfox-pico/media/isp/release_camera_engine_rkaiq_rv1106_arm-rockchip830-linux-uclibcgnueabihf/isp_iqfiles/` por `sc3336_CMK-OT2119-PC1_30IRC-F16.json`

## Generar la imagen
./build.sh clean kernel
./build.sh kernel

Revisar que salga TARGET_KERNEL_CONFIG=luckfox_rv1106_linux_defconfig 
TARGET_KERNEL_DTS=rv1103g-luckfox-pico-mini.dts
 al compilar


### Problems with CMake version
Yo tenia instalado en mi Ubuntu22.04 cmake=4.4.0, eso daba conflicto con el SDK, que espera una version inferior.
```
# Esto lo hacemos en Downloads una vez
wget https://github.com/Kitware/CMake/releases/download/v3.27.9/cmake-3.27.9-linux-x86_64.tar.gz
tar xf cmake-3.27.9-linux-x86_64.tar.gz

# Esto solo aplica a la terminal actual
export PATH=$PWD/cmake-3.27.9-linux-x86_64/bin:$PATH
cmake --version
```

## Flashear la imagen en la memoria interna de la placa


## Run program

### This is a program that raises and prepare camera at start but uses the camera so must be killed
killall rkipc
