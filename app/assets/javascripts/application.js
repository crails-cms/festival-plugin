console.log("client javascript for plugin festival RE-loaded");

const modalId = "festival-artist-modal";

function modalExists() {
  return document.querySelector(`#${modalId}`) != undefined;
}

function createArtistPopup(link, artist) {
  const root = document.createElement("div");
  const title = document.createElement("h4");
  const description = document.createElement("p");
  const picture = document.createElement("img");

  root.classList.add("popup");
  root.classList.add("festival-popup");
  root.style.top = link.offsetHeight;
  root.style.left = link.offsetWidth / 2;
  title.textContent = artist.title;
  description.textContent = artist.description;
  picture.src = artist.thumbnail_url;
  root.appendChild(picture);
  root.appendChild(title);
  root.appendChild(description);
  return root;
}

function populateArtistPage(page, artist) {
  const title = document.createElement("h2");
  const body = document.createElement("div");
  const picture = document.createElement("img");
  
  page.classList.add("body");
  title.textContent = artist.title;
  body.innerHTML = artist.body;
  picture.src = artist.thumbnail_url;
  picture.style.float = "right";
  picture.style.maxWidth = "100%";
  page.appendChild(picture);
  page.appendChild(title);
  page.appendChild(body);
}

function createExitButton() {
  const element = document.createElement("a");

  element.textContent = "x";
  element.classList.add("close");
  return element;
}

function createArtistModal(artist) {
  const root = document.createElement("div");
  const page = document.createElement("div");
  const overlay = document.createElement("div");
  const exitButton = createExitButton();
  const onDestroyEvent = function(event) {
    event.preventDefault();
    document.body.removeChild(root);
  };

  exitButton.addEventListener("click", onDestroyEvent);
  populateArtistPage(page, artist);
  overlay.classList.add("overlay");
  root.id = modalId;
  root.appendChild(overlay);
  root.appendChild(page);
  root.appendChild(exitButton);
  root.addEventListener("click", function(event) {
    const source = event.target || event.srcElement;
    if (source === overlay) {
      onDestroyEvent(event);
    }
  });
  document.body.appendChild(root);
}

class ArtistLink {
  constructor(link, artist) {
    this.root = link;
    this.root.style.position = "relative";
    this.artist = artist;
    this.root.addEventListener("mouseenter", this.onHoverOn.bind(this));
    this.root.addEventListener("mouseleave", this.onHoverOff.bind(this));
    this.root.addEventListener("click", this.onActivated.bind(this));
  }

  destroyPopup() {
    if (this.popup) {
      this.root.removeChild(this.popup);
      delete this.popup;
    }
  }

  onHoverOn(event) {
    if (event) { event.preventDefault(); }
    if (modalExists() || !this.artist.description)
      return ;
    this.popup = createArtistPopup(this.root, this.artist);
    this.root.appendChild(this.popup);
  }

  onHoverOff(event) {
    if (event) { event.preventDefault(); }
    this.destroyPopup();
  }

  onActivated(event) {
    if (modalExists())
      return ;
    console.log("ArtistLink activated", this.artist);
    this.onHoverOff(event);
    createArtistModal(this.artist);
  }
}

function createArtistLink(link, json) {
  if (!link.$controller) {
    const linkController = new ArtistLink(link, json);

    link.$controller = linkController;
    window.artistLinks.push(new ArtistLink(link, json));
  }
}

function initializeArtistLinks() {
  window.artistLinks = [];
  document.querySelectorAll(".festival-artist-link").forEach(link => {
    if (link.dataset.initialized != 1) {
      const artistId = link.dataset.id;
      const headers = new Headers();

      link.dataset.initialized = 1;
      headers.append("Accept", "application/json");
      fetch(new Request(link.href), {
        method: 'GET', headers: headers
      }).then(response => {
        return response.json();
      }).then(createArtistLink.bind(this, link));
    }
  });
}

document.addEventListener("DOMContentLoaded", initializeArtistLinks);
initializeArtistLinks();
