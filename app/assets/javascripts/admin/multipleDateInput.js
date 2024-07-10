const className = "multiple-date-time-input";
const style = `
ul.${className} {
  list-style: none;
  padding: 0.5em;
  border: 1px solid lightgray;
}
ul.${className} > li {
  display: flex;
  align-items: center;
}
ul.${className} > li:not(:last-of-type) {
  border-bottom: 1px solid lightgray;
}
ul.${className} > li > * {
  margin: 0.5em;
}`;
let styleElement;

function requireStyleElement() {
  if (!styleElement) {
    styleElement = document.createElement("style");
    styleElement.textContent = style;
    document.head.appendChild(styleElement);
  }
}

function pad(number) {
  const string = number.toString();
  return string.length == 1 ? '0' + string : string;
}

function setDate(element, timestamp) {
  const date = new Date(timestamp * 1000);
  const dateInput = element.querySelector("[type=date]");
  const timeInput = element.querySelector("[type=time]");
  const dateValue = `${date.getFullYear()}-${pad(date.getMonth() + 1)}-${pad(date.getDate())}`
  const timeValue = `${pad(date.getHours())}:${pad(date.getMinutes())}`;

  dateInput.value = dateValue;
  timeInput.value = timeValue;
  console.log("setDate", dateValue, timeValue);
}

function getTimestamp(element) {
  const dateInput = element.querySelector("[type=date]");
  const timeInput = element.querySelector("[type=time]");

  return (dateInput.valueAsNumber + timeInput.valueAsNumber) / 1000;
}

function getComment(element) {
  const input = element.querySelector("textarea");
  return input.value;
}

export default class MultipleDateTimeInput {
  constructor(input) {
    window._multipleDateTimeInput = this;
    requireStyleElement();
    this.root = document.createElement("ul");
    this.root.classList.add(className);
    this.target = input;
    this.controls = this.makeControls();
    this.root.appendChild(this.controls);
    if (input.value.length > 2)
      this.load(JSON.parse(input.value));
  }

  makeControls() {
    const element = document.createElement("li");
    const addButton = document.createElement("button");

    Cms.Style.ready.then(function() {
      Cms.Style.apply("button", addButton);
    });
    addButton.textContent = Cms.i18n.t("admin.page-editor.action.add");
    addButton.addEventListener("click", this.newDate.bind(this));
    element.appendChild(addButton);
    return element;
  }

  makeDate() {
    const element = document.createElement("li");
    const dateGroup = document.createElement("div");
    const timeGroup = document.createElement("div");
    const dateLabel = document.createElement("label");
    const timeLabel = document.createElement("label");
    const date = document.createElement("input");
    const time = document.createElement("input");
    const controls = document.createElement("div");
    const removeButton = document.createElement("button");

    Cms.Style.ready.then(function() {
      Cms.Style.apply("dangerButton", removeButton);
      Cms.Style.apply("smallButton", removeButton);
      Cms.Style.apply("formGroup", dateGroup, timeGroup);
      Cms.Style.apply("formInput", date, time);
      Cms.Style.apply("buttonGroup", controls);
    });
    dateLabel.textContent = Cms.i18n.t("admin.date");
    timeLabel.textContent = Cms.i18n.t("admin.time");
    date.type = "date";
    time.type = "time";
    removeButton.textContent = Cms.i18n.t("admin.destroy");
    dateGroup.appendChild(dateLabel);
    dateGroup.appendChild(date);
    timeGroup.appendChild(timeLabel);
    timeGroup.appendChild(time);
    controls.appendChild(removeButton);
    element.appendChild(dateGroup);
    element.appendChild(timeGroup);
    element.appendChild(controls);
    date.addEventListener("change", this.updateValue.bind(this));
    time.addEventListener("change", this.updateValue.bind(this));
    removeButton.addEventListener("click", this.removeDate.bind(this, element));
    return element;
  }

  newDate(event) {
    event.preventDefault();
    this.root.insertBefore(this.makeDate(), this.controls);
  }

  removeDate(element, event) {
    event.preventDefault();
    this.root.removeChild(element);
    this.updateValue();
  }

  load(data) {
    data.forEach(timestamp => {
      const element = this.makeDate();
      setDate(element, timestamp);
      this.root.insertBefore(element, this.controls);
    });
  }

  valueForElement(element) {
    if (element != this.controls) {
      const timestamp = getTimestamp(element);
      return !isNaN(timestamp) ? timestamp : null;
    }
    return null;
  }

  updateValue() {
    const result = [];

    this.root.querySelectorAll("li").forEach(element => {
      if (element != this.controls) {
        const value = this.valueForElement(element);

        if (value !== null)
          result.push(value);
      }
    });
    this.target.value = JSON.stringify(result);
  }
}

export class MultipleDateAndCommentInput extends MultipleDateTimeInput {
  makeDate() {
    const element = super.makeDate();
    const commentGroup = document.createElement("div");
    const commentLabel = document.createElement("label");
    const comment = document.createElement("textarea");

    Cms.Style.apply("formGroup", commentGroup);
    Cms.Style.apply("formInput", comment);
    commentLabel.textContent = Cms.i18n.t("admin.comment");
    commentGroup.appendChild(commentLabel);
    commentGroup.appendChild(comment);
    element.insertBefore(commentGroup, element.lastChild);
    comment.addEventListener("change", this.updateValue.bind(this));
    Cms.initializers.CKEditorButton(comment, { toolbar: 'compact' });
    return element;
  }

  valueForElement(element) {
    const timestamp = super.valueForElement(element);
    const comment = getComment(element);

    if (timestamp != null) {
      return comment.lenth ? [timestamp, comment] : [timestamp];
    }
    return null;
  }
}
