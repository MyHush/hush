package=rust
$(package)_version=1.16.0
$(package)_download_path=https://static.rust-lang.org/dist
$(package)_file_name=rust-$($(package)_version)-x86_64-apple-darwin.tar.gz
$(package)_sha256_hash=2d08259ee038d3a2c77a93f1a31fc59e7a1d6d1bbfcba3dba3c8213b2e5d1926

define $(package)_stage_cmds
  ./install.sh --destdir=$($(package)_staging_dir) --prefix=$(host_prefix)/native --disable-ldconfig
endef
