# Contributing To Hush

If you find a bug or have a feature request, please send a Github Pull Request
based on the `dev` branch or create a Github Issue using our issue template.

## Core Developer Workflow

This is our Hush Core Developer workflow. It is a living document that will
change as our needs change.

Three will be three main branches in hush.git:

* master     - always-buildable tip development, dev is merged into this branch (--no-ff)
* candidate  - topic branch tracking the latest release candidate
* dev        - latest tip of development, might be broken

The following branch structure will be topic branches off of the main `dev` branch,
and track platform-specific stuff for the code leading up to the vX.Y.Z release:

* vX.Y.Z-win - Windows specific stuff for the vX.Y.Z release
* vX.Y.Z-lin - Linux specific stuff for vX.Y.Z
* vX.Y.Z-mac - Mac specific stuff for the vX.Y.Z release

## How To Send Pull Requests

Only Pull Requests from candidate, for release candidates, documentation
changes and urgent hotfixes get pushed to master. Any core member may push
documentation-only changes to master without review, or urgent security
hotfixes.
