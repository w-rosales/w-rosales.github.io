let slideIndex = 1;
showSlides(slideIndex);

function plusSlides(n) {
    showSlides(slideIndex += n);
}

function showSlides(n) {
    let i;
    let slides = document.getElementsByClassName("mySlides");
    let prev = document.getElementsByClassName("prev")[0];
    let next = document.getElementsByClassName("next")[0];
    if (n > slides.length) {slideIndex = slides.length}    
    if (n < 1) {slideIndex = 1}
    for (i = 0; i < slides.length; i++) {
        slides[i].style.display = "none";  
    }
    slides[slideIndex-1].style.display = "block";  
    // Hide arrows if there are no more slides to navigate to
    prev.style.display = (slideIndex === 1) ? "none" : "block";
    next.style.display = (slideIndex === slides.length) ? "none" : "block";
}
