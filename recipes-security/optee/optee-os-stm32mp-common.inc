FILESEXTRAPATHS_prepend := "${THISDIR}/optee-os:"

PACKAGE_ARCH = "${MACHINE_ARCH}"

B = "${WORKDIR}/build"
# Configure build dir for externalsrc class usage through devtool
EXTERNALSRC_BUILD_pn-${PN} = "${WORKDIR}/build"

DEPENDS += "dtc-native python-pycrypto-native"

inherit deploy pythonnative

OPTEEMACHINE ?= "${MACHINE}"
OPTEEOUTPUTMACHINE ?= "${MACHINE}"

EXTRA_OEMAKE = "PLATFORM=${OPTEEMACHINE} \
                CROSS_COMPILE_core=${HOST_PREFIX} \
                CROSS_COMPILE_ta_arm64=${HOST_PREFIX} \
                ${@bb.utils.contains('TUNE_FEATURES', 'aarch64', 'CFG_ARM64_core=y ta-targets=ta_arm64', 'CFG_ARM32_core=y CROSS_COMPILE_ta_arm32=${HOST_PREFIX}', d)} \
                NOWERROR=1 \
                LDFLAGS= \
                LIBGCC_LOCATE_CFLAGS=--sysroot=${STAGING_DIR_HOST} \
        "

EXTRA_OEMAKE += "CFG_TEE_CORE_LOG_LEVEL=2"
EXTRA_OEMAKE += "CFG_TEE_CORE_DEBUG=n"
EXTRA_OEMAKE += "comp-cflagscore='--sysroot=${STAGING_DIR_TARGET}'"

OPTEE_ARCH_armv7a = "arm32"
OPTEE_ARCH_armv7ve = "arm32"
OPTEE_ARCH_aarch64 = "arm64"

do_configure_prepend(){
    chmod 755 ${S}/scripts/bin_to_c.py
}

do_compile() {
    unset -v CFLAGS CPPFLAGS LDFLAGS LDADD
    if [ -n "${OPTEE_CONF}" ]; then
        for conf in ${OPTEE_CONF}; do
            oe_runmake -C ${S} O=${B}/${conf} CFG_SECURE_DT=${conf}
        done
    else
        oe_runmake -C ${S} O=${B}/out
    fi
}

do_install() {
    #install TA devkit
    install -d ${D}${includedir}/optee/export-user_ta/

    if [ -n "${OPTEE_CONF}" ]; then
        for conf in ${OPTEE_CONF}; do
            for f in  ${B}/${conf}/export-ta_${OPTEE_ARCH}/* ; do
                cp -aRf  $f ${D}${includedir}/optee/export-user_ta/
            done
        done
    fi
}

# Configure optee binaries
OPTEE_BOOTCHAIN = "optee"
OPTEE_HEADER    = "tee-header_v2"
OPTEE_PAGEABLE  = "tee-pageable_v2"
OPTEE_PAGER     = "tee-pager_v2"
OPTEE_SUFFIX    = "stm32"
# Output the ELF generated
ELF_DEBUG_ENABLE ?= ""
OPTEE_ELF = "tee"
OPTEE_ELF_SUFFIX = "elf"

do_deploy() {
    install -d ${DEPLOYDIR}
    if [ -n "${OPTEE_CONF}" ]; then
        for conf in ${OPTEE_CONF}; do
            install -m 644 ${B}/${conf}/core/${OPTEE_HEADER}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_HEADER}-${conf}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
            install -m 644 ${B}/${conf}/core/${OPTEE_PAGER}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_PAGER}-${conf}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
            install -m 644 ${B}/${conf}/core/${OPTEE_PAGEABLE}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_PAGEABLE}-${conf}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
            if [ -n "${ELF_DEBUG_ENABLE}" ]; then
                install -m 644 ${B}/${conf}/core/${OPTEE_ELF}.${OPTEE_ELF_SUFFIX} ${DEPLOYDIR}/${OPTEE_ELF}-${conf}-${OPTEE_BOOTCHAIN}.${OPTEE_ELF_SUFFIX}
            fi
        done
    else
        install -m 644 ${B}/core/${OPTEE_HEADER}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_HEADER}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
        install -m 644 ${B}/core/${OPTEE_PAGER}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_PAGER}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
        install -m 644 ${B}/core/${OPTEE_PAGEABLE}.${OPTEE_SUFFIX} ${DEPLOYDIR}/${OPTEE_PAGEABLE}-${OPTEE_BOOTCHAIN}.${OPTEE_SUFFIX}
        if [ -n "${ELF_DEBUG_ENABLE}" ]; then
            install -m 644 ${B}/core/${OPTEE_ELF}.${OPTEE_ELF_SUFFIX} ${DEPLOYDIR}/${OPTEE_ELF}-${OPTEE_BOOTCHAIN}.${OPTEE_ELF_SUFFIX}
        fi
    fi
}
addtask deploy before do_build after do_compile

FILES_${PN} = "${nonarch_base_libdir}/firmware/"
FILES_${PN}-dev = "/usr/include/optee"

INSANE_SKIP_${PN}-dev = "staticdev"

INHIBIT_PACKAGE_STRIP = "1"
