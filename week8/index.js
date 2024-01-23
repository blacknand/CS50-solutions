document.addEventListener('DOMContentLoaded', function () {
    const navLinkEls = document.querySelectorAll('.nav-link');
    const windowPathName = window.location.pathname;

    navLinkEls.forEach(navLinkEl => {
        if (navLinkEl.href.includes(windowPathName)) {
            navLinkEl.classList.add('active');
        }
    });

    navLinkEls.forEach(navLinkEl => {
        navLinkEl.addEventListener('click', function () {
            // Remove 'active' class from all links
            navLinkEls.forEach(link => {
                link.classList.remove('active');
            });

            // Add 'active' class to the clicked link
            navLinkEl.classList.add('active');
        });
    });
});
