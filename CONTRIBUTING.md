# Contributing to the Templateator 2000

When contributing, make sure that the changes you wish to make are in line with the project direction. If you are not sure about this, open an issue first, so we can discuss it.

**For your first couple of PRs, start with something small to get familiar with the project and its development processes.**

Everyone is welcome to work on the project, and while we have lots of fun, it's a serious kind of fun. :^)

## Required Software

<span style="color:red">For this project, you need Visual Studio 2022, with the JetBrains Resharper C++ extension.</span>
You will need vcpkg with the following packages:

- `qt6`
- `libzippp`
- `openssl`
- `lua`
- `nlohmann-json`
- `spdlog`

## Issue policy

Please do file any bugs you find, keeping the following in mind:

- One issue per bug. Putting multiple things in the same issue makes both discussion and completion unnecessarily complicated.
- No build issues (or other support requests).
- Don't comment on issues just to add a joke or irrelevant commentary. People get notified about comments so let's keep them relevant.

## Human language policy

Here, we treat human language as seriously as we do programming language.

The following applies to all user-facing strings, code, comments, and commit messages:

- The official project language is American English with ISO 8601 dates and metric units.
- Use proper spelling, grammar, and punctuation.
- Write in an authoritative and technical tone.

Everyone is encouraged to make use of the Resharper spell checker to make this easier.

## Code submission policy

Nobody is perfect, and sometimes we mess things up. That said, here are some good dos & dont's to try and stick to:

**Do:**

- Write in C++20, using the std containers in all code.
- Choose expressive variable, function and class names. Make it as obvious as possible what the code is doing.
- Split your changes into separate, atomic commits (i.e. A commit per feature or fix, where the build, tests and the system are all functioning).
- Make sure your commits are rebased on the master branch.
- Wrap your commit messages at 72 characters.
- The first line of the commit message is the subject line, and must have the format "Category: Brief description of what's being changed". The category should be the name of a library, application, service, utility, etc.
  - Examples: `Windows`, `Scripting`, `Lua`
  - Don't use a category like "`Utilities`", except for generic changes that affect a large portion of code within these directories.
  - Don't use specific component names, e.g. C++ class names, as the category either - mention them in the summary instead.
  - Several categories may be combined with `/`, e.g. `Scripting/Windows: ...`
- Write the commit message subject line in the imperative mood ("Foo: Change the way dates work", not "Foo: Changed the way dates work").
- Write your commit messages in proper English, with care and punctuation.
- Squash your commits when making revisions after a patch review.
- Check the spelling of your code, comments and commit messages.

**Don't:**

- Touch anything outside the stated scope of the PR.
- Iterate excessively on your design across multiple commits.
- Use weasel-words like "refactor" or "fix" to avoid explaining what's being changed.
- End commit message subject lines with a period.
- Include commented-out code.
- Write in C. (Instead, take advantage of C++'s amenities, and don't limit yourself to the standard C library.)
- Attempt large architectural changes until you are familiar with the system and have worked on it for a while.
- Engage in excessive "feng shui programming" by moving code around without quantifiable benefit.

## On abandoned pull requests

Sometimes good PRs get abandoned by the author for one reason or another. If the PR is fundamentally good, but the author is not responding to requests, the PR may be manually integrated with minor changes to code and commit messages.

To make this easier, we do appreciate it if folks enable the "Allow edits from maintainers" flag on their pull requests.

## On ideologically motivated changes

This is a purely technical project. As such, it is not an appropriate arena to advertise your personal politics or religious beliefs. Any changes that appear ideologically motivated will be rejected.
