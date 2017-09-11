# Contributing To Hush

If you find a bug or have a feature request, please send a Github Pull Request based on the `dev` branch or create a Github Issue using our issue template.

# Core Developer Workflow

This is our Hush Core Developer workflow. It is a living document that will change as our needs change.

Three will be three main branches in hush.git: 

* master    - always-buildable tip development, dev is merged into this branch (--no-ff)
* candidate - topic branch tracking the latest release candidate
* dev       - latest tip of development, might be broken


Only Pull Requests from candidate, for release candidates, documentation changes and urgent hotfixes get pushed to master. Any core member may push documentation-only changes to master without review, or urgent security hotfixes.
