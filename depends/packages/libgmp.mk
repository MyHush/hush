package=libgmp
$(package)_download_path=https://github.com/joshuayabut/$(package)/archive/
$(package)_file_name=$(package)-$($(package)_git_commit).tar.gz
$(package)_download_file=$($(package)_git_commit).tar.gz
$(package)_sha256_hash=193836c1acc9dc00fe2521205d7bbe1ba13263f6cbef6f02584bf6f8b34b108f
$(package)_git_commit=053c03b1cab347671d936f43ef66b48ab5e380ee

$(package)_dependencies=
$(package)_config_opts=--enable-cxx --disable-shared

define $(package)_config_cmds
  $($(package)_autoconf) --host=$(host) --build=$(build)
endef

define $(package)_build_cmds
  $(MAKE) CPPFLAGS='-fPIC'
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install ; echo '=== staging find for $(package):' ; find $($(package)_staging_dir)
endef
