console.log("admin javascript for plugin festival loaded");

import ArtistLink from "./admin/artistLink.js";
import MultipleDateTimeInput from "./admin/multipleDateInput.js";

window.customCKEditorPlugins ||= [];
window.customCKEditorPlugins.push(ArtistLink);

function initializeFestivalEventDatesInput() {
  const input = document.querySelector("input[name=\"festival-event[dates]\"");

  if (input) {
    Promise.all([Cms.Style.ready, Cms.i18n.ready]).then(() => {
      const element = new MultipleDateTimeInput(input);

      input.parentElement.appendChild(element.root);
    });
  }
}

document.addEventListener("DOMContentLoaded", function() {
  initializeFestivalEventDatesInput();
});
initializeFestivalEventDatesInput();
