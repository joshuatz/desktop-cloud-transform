# Desktop Cloud Transform (DCT)
## Project Page
[https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/](https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/)
## What is this?
Desktop Cloud Transform (DCT) is a desktop GUI application written with QT/C++ that allows you to upload image files, and optionally apply advanced transformations on "the cloud", which are then downloaded back to your computer.

The benefit is that you can apply extremely complicated combinations of **automated** transformations and effects on the cloud instead of your computer, which means you don't need to have Photoshop installed, or deal with complex scripting.

This Git readme file is really meant to cover the technical parts of the application and how to use, rather than purpose, so for more info on the purpose and uses for this project, be sure to check out my project page [here](https://joshuatz.com/projects/applications/desktop-cloud-transform-dct/).


# Building
 - So far I've only built for Windows, but there shouldn't be any Win specific dependencies in use. On my todo list is to build for Linux and OSX and see if there are any issues with the file read/writes
 - Compile tested with MSVC and MinGW
 - Steps should be as easy as:
     1. Clone repo
     2. Open in QT Creator
     3. Build Release

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
 - Looks like QT is filelocking the image when I display it in the success modal - maybe I can clone it to a temp directory or into memory?
     - Not sure there is going to be an easy solution to this. I think this coming from using QImage (Image{} QML) natively.
         - Idea A) Copy downloaded file to temp dir (same as install or use some QT-get-sysdir type call). Delete after success modal closed, or on timer loop (or on app close or open)
         - Idea B) See if can use JS-in-QML to do something like copy image to canvas, or some other element that might force it into memory vs accessing off disk
         - Idea C) When downloading for user, read into memory as native QPixMap or something like that. Then send to QML layer as a pseudo image (not a resource URL)
 - Cleanup QNetworkReply instances - use deleteLater to free up (probs in slot?)
 - Timeout for auto-close of success modal needs to be auto-cancelled if clicked on
     - For right now, just removed auto-close entirely
## Backlog / Should implement
 - DB migrations for upgrades to app
 - Refactor naming of classes, objects, methods - distinction between transformations and configs
 - Tag items uploaded through this tool to make easier to find online
     - https://cloudinary.com/documentation/image_upload_api_reference#tags_method
 - Internal stats collection and display to user
     - Optional: API quota warning configured through globalSettings
 - Unit tests
 - Cmd line support
 - Use richtext in textarea for link - https://doc.qt.io/qt-5/richtext-html-subset.html
## Icebox / optional:
 - When uploading images, use filename to check if already uploaded, and if so, offer option to "rename" version on cloudinary sever
     - Use https://cloudinary.com/documentation/image_upload_api_reference#rename_method
 - Move some reusable JS logic stuff out of QML files and into reusable JS files
