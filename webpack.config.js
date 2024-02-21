const path = require("path");

module.exports = [
  {
    mode: "production",
    entry: "./app/assets/javascripts/admin.js",
    output: {
      path: path.resolve(__dirname, "lib/"),
      filename: "admin.js"
    },
    module: {
      rules: [
        {
          test: /[/\\]icons[/\\][^/\\]+\.svg$/,
          use: [ 'raw-loader' ]
        }
      ]
    }
  },
  {
    mode: "production",
    entry: "./app/assets/javascripts/application.js",
    output: {
      path: path.resolve(__dirname, "lib/"),
      filename: "application.js"
    }
  }
];
