.PHONY: release

release:
	@test -n "$(VERSION)" || { echo "ERROR: specify VERSION (e.g. make release VERSION=v0.2.1)"; exit 1; }
	git cliff --unreleased -t $(VERSION) > CHANGELOG.md
	git add CHANGELOG.md
	git commit -m "docs: $(VERSION)"
	git tag -a $(VERSION) -m "release $(VERSION)"
	git push && git push --tags
	gh release create $(VERSION) --notes-file CHANGELOG.md
