SUBDIRS = src

# Extra files to include in distribution
EXTRA_DIST = \
    .code2snippet.json.example \
    autogen.sh \
    README.md \
    LICENSE

# Install config directory
install-data-local:
	@$(NORMAL_INSTALL)
	$(MKDIR_P) $(DESTDIR)$(sysconfdir)/code2snippet

# Uninstall config directory
uninstall-local:
	@$(NORMAL_UNINSTALL)
	-rm -rf $(DESTDIR)$(sysconfdir)/code2snippet
