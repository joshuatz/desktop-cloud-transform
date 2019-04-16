### Notes to self:
## Docs
     - Pretty much best doc link is for "direct calls to api" - https://cloudinary.com/documentation/upload_images#uploading_with_a_direct_call_to_the_api
     - List of sections - https://cloudinary.com/documentation/cloudinary_references
     - Incoming transformations on upload - https://cloudinary.com/documentation/upload_images#incoming_transformations
### TODOS
## !! - High priority - !!
 - Timeout for auto-close of success modal needs to be auto-cancelled if clicked on
 - Add about screen modal - shows when clicking on upper right button
 - DB migrations (for install or upgrade)
## Backlog / Should implement
 - Refactor naming of classes, objects, methods - distinction between transformations and configs
 - Tag items uploaded through this tool to make easier to find online
     - https://cloudinary.com/documentation/image_upload_api_reference#tags_method
 - Internal stats collection and display to user
     - Optional: API quota warning configured through globalSettings
 - Unit tests
## Icebox / optional:
 - When uploading images, use filename to check if already uploaded, and if so, offer option to "rename" version on cloudinary sever
     - Use https://cloudinary.com/documentation/image_upload_api_reference#rename_method
