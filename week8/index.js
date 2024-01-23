document.addEventListener('DOMContentLoaded', function() {
    const navLinkEls = document.querySelectorAll('.nav-link');
    const windowPathName = window.location.pathname;

    navLinkEls.forEach(navLinkEl => {
        if (navLinkEl.href.includes(windowPathName)) {
            navLinkEl.classList.add('active');
        }
    });
});