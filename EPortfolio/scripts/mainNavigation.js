document.addEventListener("DOMContentLoaded", () => {
    const dropdownMenus = document.querySelectorAll("nav ul li ul.dropdown");
    
    dropdownMenus.forEach(menu => {
        const parent = menu.parentElement;

        // Toggle dropdown visibility on click
        parent.addEventListener("click", (e) => {
            e.stopPropagation(); // Prevent clicks from propagating to parent elements
            const isVisible = menu.style.display === "block";
            menu.style.display = isVisible ? "none" : "block";
        });
    });

    // Hide dropdowns when clicking outside
    document.addEventListener("click", () => {
        dropdownMenus.forEach(menu => {
            menu.style.display = "none";
        });
    });
});
