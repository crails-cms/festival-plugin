export default class extends Cms.Dialog {
  constructor() {
    super();
    this.root.id = "festival-artist-picker";
    this.ready = this.fetch();
  }

  open() {
    this.popup.style.overflow = "visible";
    this.popup.innerHTML = "";
    this.fetch().then(() => {
      super.open();
    });
  }

  fetch() {
    const headers = new Headers();
    const url = "/admin/artists?no_pagination=1";
    headers.append("Accept", "application/json");
    return fetch(new Request(url), {
      method: 'GET', headers: headers
    }).then(response => {
      return response.json();
    }).then(json => {
      this.artists = json;
      this.render();
    });
  }

  render() {
    const form = document.createElement("div");
    const title = document.createElement("div");
    const artistGroup = document.createElement("div");
    const label = document.createElement("label");
    const artistSelect = document.createElement("select");
    const acceptButton = document.createElement("button");

    Style.apply("form", form);
    Style.apply("formGroup", artistGroup);
    Style.apply("confirmButton", acceptButton);
    title.classList.add("popup-title");
    title.textContent = i18n.t("admin.festival.artist-picker");
    label.textContent = i18n.t("admin.festival.artist");
    this.artists.forEach(artist => {
      const option = document.createElement("option");
      option.value = artist.id;
      option.textContent = artist.name;
      artistSelect.appendChild(option);
    });
    acceptButton.textContent = "OK";
    acceptButton.addEventListener("click", () => {
      this.artistPicked(this.artistData(artistSelect.value));
    });
    artistGroup.appendChild(label);
    artistGroup.appendChild(artistSelect);
    form.appendChild(artistGroup);
    this.popup.appendChild(title);
    this.popup.appendChild(form);
    this.popup.appendChild(acceptButton);
    new Cms.TomSelect(artistSelect);
  }

  artistData(id) {
    for (let i = 0 ; i < this.artists.length ; ++i) {
      if (this.artists[i].id == id)
        return this.artists[i];
    }
    return null;
  }

  artistName(id) {
    const model = this.artistData(id);
    return model ? model.name : null;
  }
}
