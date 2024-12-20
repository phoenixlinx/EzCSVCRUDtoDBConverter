// Custom Initialization for Prism.js
document.addEventListener("DOMContentLoaded", () => {
    // Check if Prism.js is loaded
    if (typeof Prism !== "undefined") {
        // Apply syntax highlighting to all code blocks
        Prism.highlightAll();
        console.log("Prism.js loaded and initialized.");
    } else {
        console.error("Prism.js is not loaded. Ensure the CDN links are correct.");
    }
});
