# Desktop Cloud Transform (DCT)
## Project Page
[https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/](https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/)
## Pre-built release
Find under the releases tab on Github.
## What is this?
Desktop Cloud Transform (DCT) is a desktop GUI application written with QT/C++ that allows you to upload image files, and optionally apply advanced transformations on "the cloud", which are then downloaded back to your computer.

The benefit is that you can apply extremely complicated combinations of **automated** transformations and effects on the cloud instead of your computer, which means you don't need to have Photoshop installed, or deal with complex scripting.

![Demo GIF](https://github.com/joshuatz/desktop-cloud-transform/raw/master/readme-files/Desktop%20Cloud%20Transform%20(DCT)%20-%20Dog%20Demo.gif "Using various saved configurations")

You can also just use it as a drag-and-drop uploader for Cloudinary, so that you can drop a local file onto the bottom upload area and instantly get back a public URL to use anywhere you like.

This Git readme file is really meant to cover the technical parts of the application and how to use it, rather than purpose, so for more info on the background and uses for this project, be sure to check out my project page [here](https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/).

# Disclaimers:
First, this was built mainly for myself, most over the course of a week, and as a way to get up to speed on some QT/C++ stuff. Please don't view it as production-level quality code.

Also, although DCT makes extensive use of Cloudinary and its API, it is not affiliated with Cloudinary in any official capacity. I am not responsible for how you use their services, and it is up to you as a user to know their TOS and be responsible with your usage of their services while using my application.

# How to use it
## GUI
![Main User Interface](https://github.com/joshuatz/desktop-cloud-transform/raw/master/readme-files/Desktop%20Cloud%20Transform%20-%20GUI%20Diagram.png "Using various saved configurations")
## What are DCT "Configurations"?
A configuration is a set of DCT settings (such as how to save the resulting file) and Cloudinary transformation settings (such as adding overlays, modifying colors, etc.). Once you have created a configuration, you can drag and drop a file into its row to upload your image, have the config settings applied, and the resulting image file downloaded back to your hard drive. There are a few preloaded configs so you can get a feel for how they work. You can delete or modify configs at any time.
## Macro support
Macros are case-insensitive; I'm using camelcase below just to make it easier to read. Macros should always be enclosed in braces, with no spaces. Certain transformations are only supported on "outgoing transformations", which are transformations that are applied **after** an image has been uploaded.

Macro | Incoming Transformation | Outgoing Transformation | What it does
--- | --- | --- | ---
{uploaded} | No | Yes | Inserts public ID as overlay into chain - "l_PUBLICID"
{uploadedId} | No | Yes | Inserts public ID, *as-is*
{width} | Yes | Yes | Inserts width in pixels of local file being uploaded
{height} | Yes | Yes | Inserts height in pixels of local file being uploaded
{filename} | Yes | Yes | Inserts the filename *including* the extension
{filenameNoExt} or {basename} | Yes | Yes | Inserts the filename *without* the extension
{createdStamp} | Yes | Yes | Inserts the timestamp of when the uploaded file was created, in secs since epoch
{cloudinaryCloudName} | Yes | Yes | Inserts *your* cloudinary cloud name / id
{timestamp} | Yes | Yes | Inserts the current time, as secs since epoch

# Building
First, note that this contains git *submodules*, so you either need to clone with `git clone --recursive`, or use `git submodule update --init` after cloning.

Actual building:
 - So far I've only built for Windows, but there shouldn't be any Win specific dependencies in use. On my todo list is to build for Linux and OSX and see if there are any issues with the file read/writes
 - Compile tested with MSVC. Should work with MinGW too.
 - Steps should be as easy as:
     1. Clone repo
     2. Open in QT Creator
     3. Build Release
 - Make sure you have openSSL in your system path

# Deploying
I'm using windeployqt to pull in DLLs to bundle with my exe for release. however, it doesn't pull in OpenSSL although it is needed for the Cloudinary API calls.

# License
Licensed under Apache 2.0
See LICENSE file for details.

# Notes to self:
## Stats logging
### Syntax:
```
Stats::getInstance()->logStat(QString category, QString action,bool usedCloudinary);
```
Example:
```
Stats::getInstance()->logStat("application","startup",false);
```
### Storage
 - Stats are stored with id as primary key (auto-increment) and column with timestamp, as sec since epoch (auto generated by Stats class)
### Established combos
Row | Category | Action | Uses Cloudinary (credits)
--- | --- | --- | ---
1 | application | upload | false
2 | cloudinary | upload | true
3 | cloudinary | transform | true
4 | cloudinary | store | true
5 | application | download | false
6 | cloudinary | download | true
### Considering Cloudinary Quotas
 - A single action (uploading an image) can trigger many events, some of which can count against a quota. For example
     - Uploading an image, and store original = true, and downloading the final transformed image
         - This would use rows 1 & 5
         - This would also use credit rows 2, 3, 4, & 6
         - Currently, the way I'm doing this it only uses credit row 4 (storing an asset) once, since it stores the original, and then to get the transformed version, it simply fetches the original public URL + transformation string. The transformed version is not actually stored on Cloudinary - it is just a derived asset.
## Docs
 - Pretty much best doc link is for "direct calls to api" - https://cloudinary.com/documentation/upload_images#uploading_with_a_direct_call_to_the_api
 - List of sections - https://cloudinary.com/documentation/cloudinary_references
 - Incoming transformations on upload - https://cloudinary.com/documentation/upload_images#incoming_transformations
# TODOS
## !! - High priority - !!
 - Add support for [*signed URLs*](https://cloudinary.com/documentation/control_access_to_media#signed_delivery_urls) (necessary for downloading via transformation URL, if user has enabled ["strict transformations"](https://cloudinary.com/documentation/control_access_to_media#strict_transformations))
 - Fix help tooltip overflow
 - Preserve filename on regular uploads (right now, it is generating a new unique ID, regardless of filename)
 - Add "duplicate" button, to clone existing configs and modify
 - Work on build script for creating deployment releases, and try to reduce output size
 - ~~Timeout for auto-close of success modal needs to be auto-cancelled if clicked on~~
     - For right now, just removed auto-close entirely
## Backlog / Should implement
 - Highlight transformation string on click for easy copy and paste
 - DB migrations for upgrades to app
 - Refactor naming of classes, objects, methods - distinction between transformations and configs
 - Tag items uploaded through this tool to make easier to find online
     - https://cloudinary.com/documentation/image_upload_api_reference#tags_method
 - Internal stats collection and display to user
     - Optional: API quota warning configured through globalSettings
 - Unit tests
 - Cmd line support
 - Use richtext in textarea for link - https://doc.qt.io/qt-5/richtext-html-subset.html
 - Add support for more hosts (Imgur anonymous upload as top priority)
## Icebox / optional:
 - When uploading images, use filename to check if already uploaded, and if so, offer option to "rename" version on cloudinary sever
     - Use https://cloudinary.com/documentation/image_upload_api_reference#rename_method
 - Move some reusable JS logic stuff out of QML files and into reusable JS files
