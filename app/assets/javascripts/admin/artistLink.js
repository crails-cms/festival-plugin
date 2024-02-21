import artistsIcon from '../../icons/artists.svg';
import ArtistPicker from "./artistPicker.js";

const ckElementId = "festivalArtistLink";

class Command extends Cms.CKEditor.Command {
  execute({ value, label }) {
    const editor = this.editor;
    const selection = editor.model.document.selection;

    editor.model.change(writer => {
      const link = writer.createElement(ckElementId, {
        ...Object.fromEntries(selection.getAttributes()),
        label: label,
        artist: value
      });
      editor.model.insertObject(link, null, null, { setSelection: 'on' });
    });
  }

  refresh() {
    const model = this.editor.model;
    const selection = model.document.selection;
    const isAllowed = model.schema.checkChild(selection.focus.parent, ckElementId);
    this.isEnabled = isAllowed;
  }
}

class Ui extends ArtistPicker {
  constructor(artistEditor) {
    super();
    this.editor = artistEditor.editor;
    this.editor.ui.componentFactory.add(
      artistEditor.constructor.toolName,
      this.createToolbarButton.bind(this)
    );
  }
  
  createToolbarButton() {
    const button = new Cms.CKEditor.ButtonView();
    button.set({label: "Artist link", icon: artistsIcon });
    button.on("execute", this.open.bind(this));
    return button;
  }

  artistPicked(model) {
    const editor = this.editor;
    editor.execute(ckElementId, { value: model.id, label: model.name });
    editor.editing.view.focus();
    this.close();
  }
}

export default class extends Cms.CKEditor.Plugin {
  static toolName = "festivalArtist";

  init() {
    this.userInterface = new Ui(this);
    this.defineSchema();
    this.defineConverters();
    this.editor.commands.add(ckElementId, new Command(this.editor));
  }

  defineSchema() {
    const schema = this.editor.model.schema;

    schema.register(ckElementId, {
      inheritAllFrom: '$inlineObject',
      allowAttributes: ['artist', 'label']
    });
  }

  defineConverters() {
    const editor = this.editor;
    const conversion = editor.conversion;

    conversion.for("upcast").elementToElement({
      view: { name: "a", classes: ["festival-artist-link"] },
      model: (viewElement, { writer: modelWriter }) => {
        return this.createModel(viewElement, modelWriter);
      }
    });
    conversion.for("editingDowncast").elementToElement({
      model: ckElementId,
      view: (modelItem, { writer: viewWriter }) => {
        return Cms.CKEditor.toWidget(
          this.createElement(modelItem, viewWriter),
          viewWriter
        );
      }
    })
    conversion.for("dataDowncast").elementToElement({
      model: ckElementId,
      view: (modelItem, { writer: viewWriter }) => {
        return this.createElement(modelItem, viewWriter);
      }
    });
  }

  createModel(viewElement, modelWriter) {
    const label = viewElement.getChild(0).data.slice();
    return modelWriter.createElement(ckElementId, {
      artist: viewElement.getAttribute("data-id"),
      label: label
    });
  }

  createElement(modelElement, viewWriter) {
    const artist = modelElement.getAttribute("artist");
    const label = modelElement.getAttribute("label");
    const view = viewWriter.createContainerElement("a", {
      class:     "festival-artist-link",
      "href":    `/festival/artists/by-id/${artist}`,
      "data-id": artist
    });
    const innerText = viewWriter.createText(label);

    window.poil = viewWriter;
    window.bitepoil = view;
    viewWriter.insert(
      viewWriter.createPositionAt(view, 0),
      innerText
    );
    return view;
  }
}
